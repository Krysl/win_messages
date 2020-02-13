// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

import 'package:flutter/services.dart';

import 'win_type.dart';

/// The name of the plugin's platform channel.
const String _winMessagesChannelName = 'flutter/win_messages';

const String _getWindowInfoMethod = 'SendMessage';

/// A singleton object that handles the interaction with the platform channel.
class WinMessagesChannel {
  /// Private constructor.
  WinMessagesChannel._();

  final MethodChannel _platformChannel =
      const MethodChannel(_winMessagesChannelName);

  /// The static instance of the menu channel.
  static final WinMessagesChannel instance = new WinMessagesChannel._();

  /// this use windows's API:
  /// LRESULT SendMessage(
  ///   HWND   hWnd,
  ///   UINT   Msg,
  ///   WPARAM wParam,
  ///   LPARAM lParam
  /// );
  void sendWinMessage(UINT msg, [WPARAM wParam, LPARAM lParam]) async {
    await _platformChannel.invokeMethod(_getWindowInfoMethod,
        [msg.toUint8List(), wParam?.toUint8List(), lParam?.toUint8List()]);
  }
}
