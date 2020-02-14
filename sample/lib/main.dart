// Copyright 2018 Google LLC
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

import 'package:flutter/foundation.dart'
    show debugDefaultTargetPlatformOverride;
import 'package:flutter/material.dart';
import 'package:window_size/window_size.dart';
import 'package:win_messages/win_messages.dart';

import 'titlebar_widgets.dart';

void main() {
  // See https://github.com/flutter/flutter/wiki/Desktop-shells#target-platform-override
  debugDefaultTargetPlatformOverride = TargetPlatform.fuchsia;
  runApp(new MyApp());
}

class MyApp extends StatelessWidget {
  final String title = 'Win_messages Demo';
  @override
  Widget build(BuildContext context) {
    setWindowTitle(title);
    // setWindowFrame(Rect.fromLTWH(100.0, 200.0, 800.0, 600.0));
    return MaterialApp(
      title: title,
      themeMode: ThemeMode.dark,
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
        // See https://github.com/flutter/flutter/wiki/Desktop-shells#fonts
        fontFamily: 'SourceHanSansCN', //'Roboto',
        buttonTheme: ButtonThemeData(
          minWidth: 5,
        ),
      ),
      home: MyHomePage(title: title),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  PlatformWindow windowInfo;
  String loginfo = 'uninited';
  Offset dragStartPos;
  Offset lastOfst = Offset.zero;
  Rect dragStartFrame;
  String dropdownValue = 'One';
  static const double fontsize = 15;
  bool maximized = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      // backgroundColor: Colors.black,
      body: GestureDetector(
        onHorizontalDragStart: (details) {
          setState(() {
            getWindowInfo().then((value) {
              windowInfo = value;
              dragStartFrame = windowInfo.frame;
            });
            loginfo = '<S>' + details.toString();
            dragStartPos = details.localPosition;
          });
        },
        onHorizontalDragUpdate: (details) {
          setState(() {
            getWindowInfo().then((value) {
              windowInfo = value;
              Rect frame = windowInfo.frame;
              // if (lastOfst.distance < 2) lastOfst = details.localPosition;
              Offset ofst = Offset((details.localPosition.dx - dragStartPos.dx),
                  (details.localPosition.dy - dragStartPos.dy));
              lastOfst = ofst;
              // if (ofst.distance < 2) return;
              Rect newFrame = Rect.fromLTRB(
                  frame.left + ofst.dx,
                  frame.top + ofst.dy,
                  frame.right + ofst.dx,
                  frame.bottom + ofst.dy);
              // print(loginfo);
              setWindowFrame(newFrame);
              loginfo = '<U>' +
                  details.globalPosition.toString() +
                  ',' +
                  details.delta.toString() +
                  ',' +
                  ofst.toString();
            });
          });
        },
        child: Column(
          children: <Widget>[
            Container(
              // color: Colors.black,
              constraints: BoxConstraints.tightForFinite(height: 40),
              child: Row(
                children: <Widget>[
                  Row(
                    children: <Widget>[
                      TitleButton(
                          onPressed: () {},
                          width: 45,
                          child: Image.asset('images/WWCQ3.png')),
                      TitleButton(
                        onPressed: () {},
                        // width: 20,
                        fontsize: fontsize,
                        text: 'File',
                      ),
                      TitleButton(
                        onPressed: () {},
                        // width: 20,
                        fontsize: fontsize,
                        text: 'Edit',
                      ),
                      TitleButton(
                        onPressed: () {},
                        // width: 20,
                        fontsize: fontsize,
                        text: 'Select',
                      ),
                    ],
                  ),
                  Expanded(
                    child: Container(
                      color: Colors.black,
                      alignment: Alignment.center,
                      constraints: BoxConstraints.expand(),
                      child: Text(
                        widget.title,
                        style: TextStyle(fontSize: 20, color: Colors.white),
                      ),
                    ),
                  ),
                  Row(
                    children: <Widget>[
                      TitleButton(
                        onPressed: () {
                          sendWinMessage(WinMessage.SYSCOMMAND('SC_MINIMIZE'));
                        },
                        text: '—',
                        child: Icon(
                          Icons.remove,
                          color: Colors.white,
                        ),
                      ),
                      TitleButton(
                        onPressed: () {
                          if (maximized == false)
                            sendWinMessage(
                                WinMessage.SYSCOMMAND('SC_MAXIMIZE'));
                          else
                            sendWinMessage(WinMessage.SYSCOMMAND('SC_RESTORE'));
                          setState(() {
                            maximized = !maximized;
                          });
                        },
                        text: '□',
                        child: Icon(
                          (maximized == false)
                              ? Icons.fullscreen
                              : Icons.fullscreen_exit,
                          color: Colors.white,
                        ),
                      ),
                      TitleButton(
                        onPressed: () {
                          sendWinMessage(WinMessage.CLOSE());
                        },
                        text: '×',
                        hoverColor: Colors.red,
                        child: Icon(
                          Icons.close,
                          color: Colors.white,
                        ),
                      ),
                    ],
                  ),
                ],
              ),
            ),
            Expanded(
              child: Container(
                color: Color.fromRGBO(20, 20, 20, 1),
                alignment: Alignment.center,
                constraints: BoxConstraints.expand(),
                child: Text(
                  'Body',
                  style: TextStyle(fontSize: 20, color: Colors.blue),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
