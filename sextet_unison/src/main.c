// UNISON SextetStream bridge.
//
// Background tray app that receives the lights from ITGMania/StepMania 5
// SextetStreamToFile lights driver through a named pipe and forwards them
// to the connected UNISON boards. The first board drives the P1 pad and
// the second board drives the P2 pad (can be swapped from the tray menu).
//
// Preferences.ini:
//   LightsDriver=SextetStreamToFile
//   SextetStreamOutputFilename=\\.\pipe\StepMania-Lights-SextetStream

#define _WIN32_WINNT 0x0601
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <unison.h>

#define APP_NAME "UNISON Sextet"
#define APP_MUTEX_NAME "Local\\BlueDotUnisonSextet"
#define PIPE_NAME "\\\\.\\pipe\\StepMania-Lights-SextetStream"
#define PIPE_BUFFER_SIZE 4096

#define MAX_BOARDS 2
#define BOARD_RESCAN_INTERVAL_MS 2000

// A SextetStream line is 13 data characters followed by LF, see
// ITGMania src/arch/Lights/LightsDriver_SextetStream.md
#define SEXTET_LINE_SIZE 13
#define SEXTET_CAB 0
#define SEXTET_P1_BUTTONS 1
#define SEXTET_P1_PAD 3
#define SEXTET_P2_BUTTONS 7
#define SEXTET_P2_PAD 9

#define WM_APP_TRAY (WM_APP + 1)
#define WM_APP_STATUS (WM_APP + 2)

#define ID_TRAY_ICON 1
#define ID_MENU_STATUS 100
#define ID_MENU_SWAP 101
#define ID_MENU_EXIT 102

typedef struct {
  bd_unison_lights_t players[2];  // Same cab and buttons, pad of each player
} lights_state_t;

static HWND g_hwnd = NULL;
static UINT g_taskbar_created_msg = 0;
static NOTIFYICONDATAA g_tray;
static char g_ini_path[MAX_PATH];
static char g_status[128] = "Starting";

static volatile LONG g_running = 1;
static volatile LONG g_swap_players = 0;

// Owned by the pipe thread
static bd_unison_device_t *g_boards[MAX_BOARDS];
static int g_board_count = 0;
static DWORD g_last_scan = 0;
static int g_game_connected = 0;
static lights_state_t g_lights;

// --- Boards ---

static void CloseBoards(void) {
  for(int i = 0; i < g_board_count; i++) {
    bd_unison_close_device(g_boards[i]);
  }
  g_board_count = 0;
}

static void PostStatus(void) {
  PostMessage(g_hwnd, WM_APP_STATUS, (WPARAM)g_game_connected, (LPARAM)g_board_count);
}

static void WriteBoards(void) {
  for(int i = 0; i < g_board_count; i++) {
    int player = (i + g_swap_players) % 2;
    if(bd_unison_write_lights(g_boards[i], &g_lights.players[player]) != 0) {
      // A board was disconnected, reopen the boards on the next update.
      CloseBoards();
      g_last_scan = 0;
      PostStatus();
      return;
    }
  }
}

static void RefreshBoards(int force) {
  DWORD now = GetTickCount();
  if(!force && g_last_scan != 0 && now - g_last_scan < BOARD_RESCAN_INTERVAL_MS) {
    return;
  }
  g_last_scan = now;

  int connected = bd_unison_count_devices();
  if(connected > MAX_BOARDS) {
    connected = MAX_BOARDS;
  }
  if(connected == g_board_count) {
    return;
  }

  CloseBoards();
  g_board_count = bd_unison_open_devices(g_boards, MAX_BOARDS);
  WriteBoards();
  PostStatus();
}

// --- SextetStream ---

static uint8_t MapButtons(uint8_t sextet) {
  uint8_t buttons = 0;
  if(sextet & 0x01) buttons |= BD_UNISON_LIGHT_BTN_MENU_LEFT;
  if(sextet & 0x02) buttons |= BD_UNISON_LIGHT_BTN_MENU_RIGHT;
  if(sextet & 0x04) buttons |= BD_UNISON_LIGHT_BTN_MENU_UP;
  if(sextet & 0x08) buttons |= BD_UNISON_LIGHT_BTN_MENU_DOWN;
  if(sextet & 0x10) buttons |= BD_UNISON_LIGHT_BTN_START;
  return buttons;
}

// Dance mapping: #1 left, #2 right, #3 up, #4 down.
static uint8_t MapPad(uint8_t sextet) {
  uint8_t pad = 0;
  if(sextet & 0x01) pad |= BD_UNISON_LIGHT_PAD_LEFT;
  if(sextet & 0x02) pad |= BD_UNISON_LIGHT_PAD_RIGHT;
  if(sextet & 0x04) pad |= BD_UNISON_LIGHT_PAD_UP;
  if(sextet & 0x08) pad |= BD_UNISON_LIGHT_PAD_DOWN;
  return pad;
}

static void HandleLine(const uint8_t *line) {
  uint8_t cab_sextet = line[SEXTET_CAB] & 0x3F;
  uint8_t cab = 0;
  if(cab_sextet & 0x01) cab |= BD_UNISON_LIGHT_CAB_MARQUEE_UP_LEFT;
  if(cab_sextet & 0x02) cab |= BD_UNISON_LIGHT_CAB_MARQUEE_UP_RIGHT;
  if(cab_sextet & 0x04) cab |= BD_UNISON_LIGHT_CAB_MARQUEE_LR_LEFT;
  if(cab_sextet & 0x08) cab |= BD_UNISON_LIGHT_CAB_MARQUEE_LR_RIGHT;
  if(cab_sextet & 0x10) cab |= BD_UNISON_LIGHT_CAB_BASS_LEFT;
  if(cab_sextet & 0x20) cab |= BD_UNISON_LIGHT_CAB_BASS_RIGHT;

  uint8_t p1_buttons = MapButtons(line[SEXTET_P1_BUTTONS] & 0x3F);
  uint8_t p2_buttons = MapButtons(line[SEXTET_P2_BUTTONS] & 0x3F);

  for(int player = 0; player < 2; player++) {
    g_lights.players[player].cab = cab;
    g_lights.players[player].p1_buttons = p1_buttons;
    g_lights.players[player].p2_buttons = p2_buttons;
  }
  g_lights.players[0].pad = MapPad(line[SEXTET_P1_PAD] & 0x3F);
  g_lights.players[1].pad = MapPad(line[SEXTET_P2_PAD] & 0x3F);

  RefreshBoards(0);
  WriteBoards();
}

static void ReadLines(HANDLE pipe) {
  uint8_t chunk[256];
  uint8_t line[64];
  size_t line_len = 0;
  DWORD read;

  while(g_running && ReadFile(pipe, chunk, sizeof(chunk), &read, NULL) && read > 0) {
    for(DWORD i = 0; i < read; i++) {
      if(chunk[i] == '\n') {
        if(line_len >= SEXTET_LINE_SIZE) {
          HandleLine(line);
        }
        line_len = 0;
      } else if(line_len < sizeof(line)) {
        line[line_len++] = chunk[i];
      }
    }
  }
}

static DWORD WINAPI PipeThread(LPVOID param) {
  (void)param;

  while(g_running) {
    HANDLE pipe = CreateNamedPipeA(
        PIPE_NAME,
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,                  // single instance
        0,                  // out buffer
        PIPE_BUFFER_SIZE,   // in buffer
        0,                  // default timeout
        NULL);              // default security

    if(pipe == INVALID_HANDLE_VALUE) {
      Sleep(1000);
      continue;
    }

    RefreshBoards(1);
    PostStatus();

    // The game connects to the pipe only once at startup.
    if(ConnectNamedPipe(pipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
      g_game_connected = 1;
      PostStatus();
      ReadLines(pipe);
      g_game_connected = 0;
    }

    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);

    // Turn the lights off when the game exits.
    memset(&g_lights, 0, sizeof(g_lights));
    WriteBoards();
    PostStatus();
  }

  return 0;
}

// --- Settings ---

static void InitSettings(void) {
  DWORD len = GetModuleFileNameA(NULL, g_ini_path, sizeof(g_ini_path));
  char *slash = strrchr(g_ini_path, '\\');
  if(len == 0 || slash == NULL) {
    strcpy(g_ini_path, ".\\");
    slash = g_ini_path + 1;
  }
  slash[1] = '\0';
  strncat(g_ini_path, "UnisonSextet.ini", sizeof(g_ini_path) - strlen(g_ini_path) - 1);

  g_swap_players = GetPrivateProfileIntA("Settings", "SwapPlayers", 0, g_ini_path) ? 1 : 0;
}

static void SaveSettings(void) {
  WritePrivateProfileStringA("Settings", "SwapPlayers", g_swap_players ? "1" : "0", g_ini_path);
}

// --- Tray ---

static void UpdateTray(DWORD flags) {
  snprintf(g_tray.szTip, sizeof(g_tray.szTip), "%s: %s", APP_NAME, g_status);
  Shell_NotifyIconA(flags, &g_tray);
}

static void AddTray(void) {
  memset(&g_tray, 0, sizeof(g_tray));
  g_tray.cbSize = sizeof(g_tray);
  g_tray.hWnd = g_hwnd;
  g_tray.uID = ID_TRAY_ICON;
  g_tray.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  g_tray.uCallbackMessage = WM_APP_TRAY;
  g_tray.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  UpdateTray(NIM_ADD);
}

static void ShowMenu(void) {
  HMENU menu = CreatePopupMenu();
  AppendMenuA(menu, MF_STRING | MF_GRAYED, ID_MENU_STATUS, g_status);
  AppendMenuA(menu, MF_SEPARATOR, 0, NULL);
  AppendMenuA(menu, MF_STRING | (g_swap_players ? MF_CHECKED : 0), ID_MENU_SWAP, "Swap P1/P2 boards");
  AppendMenuA(menu, MF_STRING, ID_MENU_EXIT, "Exit");

  POINT cursor;
  GetCursorPos(&cursor);
  SetForegroundWindow(g_hwnd);
  TrackPopupMenu(menu, TPM_RIGHTBUTTON, cursor.x, cursor.y, 0, g_hwnd, NULL);
  PostMessage(g_hwnd, WM_NULL, 0, 0);
  DestroyMenu(menu);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_APP_TRAY:
    if(lparam == WM_RBUTTONUP || lparam == WM_LBUTTONUP) {
      ShowMenu();
    }
    return 0;
  case WM_APP_STATUS: {
    int boards = (int)lparam;
    snprintf(g_status, sizeof(g_status), "%s, %d board%s",
             wparam ? "game connected" : "waiting for game",
             boards, boards == 1 ? "" : "s");
    UpdateTray(NIM_MODIFY);
    return 0;
  }
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case ID_MENU_SWAP:
      InterlockedExchange(&g_swap_players, !g_swap_players);
      SaveSettings();
      return 0;
    case ID_MENU_EXIT:
      DestroyWindow(hwnd);
      return 0;
    }
    break;
  case WM_DESTROY:
    Shell_NotifyIconA(NIM_DELETE, &g_tray);
    PostQuitMessage(0);
    return 0;
  default:
    // Explorer restarted, add the tray icon again.
    if(msg == g_taskbar_created_msg && g_taskbar_created_msg != 0) {
      AddTray();
      return 0;
    }
    break;
  }
  return DefWindowProcA(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show) {
  (void)prev_instance;
  (void)cmd_line;
  (void)show;

  HANDLE mutex = CreateMutexA(NULL, TRUE, APP_MUTEX_NAME);
  if(mutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS) {
    MessageBoxA(NULL, APP_NAME " is already running.", APP_NAME, MB_OK | MB_ICONINFORMATION);
    return 0;
  }

  InitSettings();

  WNDCLASSA wc;
  memset(&wc, 0, sizeof(wc));
  wc.lpfnWndProc = WndProc;
  wc.hInstance = instance;
  wc.lpszClassName = "BlueDotUnisonSextet";
  RegisterClassA(&wc);

  g_hwnd = CreateWindowA(wc.lpszClassName, APP_NAME, 0, 0, 0, 0, 0, NULL, NULL, instance, NULL);
  if(g_hwnd == NULL) {
    return 1;
  }

  g_taskbar_created_msg = RegisterWindowMessageA("TaskbarCreated");
  AddTray();

  HANDLE thread = CreateThread(NULL, 0, PipeThread, NULL, 0, NULL);

  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Stop the pipe thread, it may be blocked waiting for the game.
  InterlockedExchange(&g_running, 0);
  int stopped = 1;
  if(thread != NULL) {
    for(int i = 0; i < 20 && WaitForSingleObject(thread, 100) == WAIT_TIMEOUT; i++) {
      CancelSynchronousIo(thread);
    }
    stopped = WaitForSingleObject(thread, 0) == WAIT_OBJECT_0;
    CloseHandle(thread);
  }

  // Boards are owned by the pipe thread, touch them only once it stopped.
  if(stopped) {
    memset(&g_lights, 0, sizeof(g_lights));
    WriteBoards();
    CloseBoards();
  }

  CloseHandle(mutex);
  return 0;
}
