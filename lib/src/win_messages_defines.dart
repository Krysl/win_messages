// ignore_for_file: non_constant_identifier_names
import 'package:win_messages/win_messages.dart';

final int WM_SIZE = 0x0005;
final int WM_CLOSE = 0x0010;
final int WM_SYSCOMMAND = 0x0112;

class WinMessage {
  // static final UINT SIZE = UINT(WM_SIZE);
  // static final UINT CLOSE = UINT(WM_CLOSE);
  UINT msg;
  WPARAM wParam;
  LPARAM lParam;
  WinMessage.CLOSE() {
    msg = UINT(WM_CLOSE);
  }

  /// See: https://docs.microsoft.com/en-us/windows/win32/menurc/wm-syscommand
  /// lParam
  /// The low-order word specifies the horizontal position of the cursor, in screen coordinates, if a window menu command is chosen with the mouse. Otherwise, this parameter is not used.
  /// The high-order word specifies the vertical position of the cursor, in screen coordinates, if a window menu command is chosen with the mouse. This parameter is 1 if the command is chosen using a system accelerator, or zero if using a mnemonic.
  WinMessage.SYSCOMMAND(String cmd, [LPARAM lparam]) {
    Map<String, int> cmdMap = {
      'SC_CLOSE': 0xF060, //Closes the window.
      'SC_CONTEXTHELP':
          0xF180, //Changes the cursor to a question mark with a pointer. If the user then clicks a control in the dialog box, the control receives a WM_HELP message.
      'SC_DEFAULT':
          0xF160, //Selects the default item; the user double-clicked the window menu.
      'SC_HOTKEY':
          0xF150, //Activates the window associated with the application-specified hot key. The lParam parameter identifies the window to activate.
      'SC_HSCROLL': 0xF080, //Scrolls horizontally.
      'SCF_ISSECURE':
          0x00000001, //Indicates whether the screen saver is secure.
      'SC_KEYMENU':
          0xF100, //Retrieves the window menu as a result of a keystroke. For more information, see the Remarks section.
      'SC_MAXIMIZE': 0xF030, //Maximizes the window.
      'SC_MINIMIZE': 0xF020, //Minimizes the window.
      'SC_MONITORPOWER':
          0xF170, // Sets the state of the display. This command supports devices that have power-saving features, such as a battery-powered personal computer.
      // The lParam parameter can have the following values:
      // -1 (the display is powering on)
      // 1 (the display is going to low power)
      // 2 (the display is being shut off)
      'SC_MOUSEMENU':
          0xF090, //Retrieves the window menu as a result of a mouse click.
      'SC_MOVE': 0xF010, //Moves the window.
      'SC_NEXTWINDOW': 0xF040, //Moves to the next window.
      'SC_PREVWINDOW': 0xF050, //Moves to the previous window.
      'SC_RESTORE':
          0xF120, //Restores the window to its normal position and size.
      'SC_SCREENSAVE':
          0xF140, //Executes the screen saver application specified in the [boot] section of the System.ini file.
      'SC_SIZE': 0xF000, //Sizes the window.
      'SC_TASKLIST': 0xF130, //Activates the Start menu.
      'SC_VSCROLL': 0xF070, //Scrolls vertically.
    };
    msg = UINT(WM_SYSCOMMAND);
    wParam = WPARAM(cmdMap[cmd]);
    lParam = lparam;
  }
}
