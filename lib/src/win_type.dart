import 'dart:typed_data';

/// used for windows's API:
/// LRESULT SendMessage(
///   HWND   hWnd,
///   UINT   Msg,
///   WPARAM wParam,
///   LPARAM lParam
/// );

class FixLenBytes {
  Uint8List _data;
  FixLenBytes.fromInt({int bitlen, int val}) {
    _data = Uint8List(bitlen >> 3);
    for (var i = 0; i < bitlen >> 3; i++) {
      _data[i] = val & 0xFF;
      val >>= 8;
    }
  }
  Uint8List toUint8List() => _data;
}

// unsigned int32
class UINT extends FixLenBytes {
  static int bitlen = 32;
  UINT(int val) : super.fromInt(bitlen: bitlen, val: val);
}

//unsigned int64
class WPARAM extends FixLenBytes {
  static int bitlen = 64;
  WPARAM(int val) : super.fromInt(bitlen: bitlen, val: val);
}

//unsigned int64
class LPARAM extends FixLenBytes {
  static int bitlen = 64;
  LPARAM(int val) : super.fromInt(bitlen: bitlen, val: val);
}
