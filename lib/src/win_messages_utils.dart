import 'win_messages_channel.dart';

void sendWinMessage(int msg) async {
  return await WinMessagesChannel.instance.sendWinMessage(msg);
}
