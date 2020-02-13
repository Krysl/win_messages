import 'win_messages_channel.dart';
import 'win_messages_defines.dart';

void sendWinMessage(WinMessage msg) async {
  WinMessagesChannel.instance.sendWinMessage(msg.msg, msg.wParam, msg.lParam);
}
