


import 'package:flutter_test/flutter_test.dart';
import 'package:win_messages/src/win_type.dart';

void main() {
  group('UINT', () {
    test('UINT test', () {
      UINT uint = UINT(0x12345678);
      expect(uint.toUint8List().toList(), [0x78, 0x56, 0x34, 0x12]);
    });
  });
  
}