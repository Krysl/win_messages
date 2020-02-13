import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

class TitleButton extends StatelessWidget {
  final VoidCallback onPressed;
  final double fontsize;
  final Color frontColor;
  final Color color;
  final Color hoverColor;
  final Widget child;
  final String text;
  final double width;
  final EdgeInsetsGeometry padding;
  TitleButton({
    @required this.onPressed,
    this.fontsize = 20,
    this.frontColor = Colors.white,
    this.color = Colors.black,
    this.hoverColor = const Color.fromRGBO(40, 40, 40, 1),
    this.child,
    this.text,
    this.width = 50,
    this.padding = const EdgeInsets.symmetric(vertical: 0, horizontal: 8),
  });

  @override
  Widget build(BuildContext context) {
    Widget contain = child;
    if (child == null) {
      contain = Text(
        text,
        style: TextStyle(
          fontSize: fontsize,
          color: frontColor,
        ),
      );
    }
    return RaisedButton(
      shape: Border(),
      color: color,
      hoverColor: hoverColor,
      onPressed: onPressed,
      padding: padding,
      materialTapTargetSize: MaterialTapTargetSize.shrinkWrap,
      child: Container(
        // color: Color.fromRGBO(255, 255, 255, 0.15),
        padding: const EdgeInsets.all(0),
        alignment: Alignment.center,
        margin: const EdgeInsets.all(0),
        // constraints: BoxConstraints.tightForFinite(width: width),
        constraints: BoxConstraints(
            minWidth: 0,
            maxWidth: width??double.infinity,
            minHeight: 0,
            maxHeight: double.infinity),
        child: contain,
      ),
    );
  }
}
