import 'package:flutter/material.dart';

class ValueUnitWidget extends StatelessWidget {
  final double value;
  final String unit;
  // Optional function to provide color based on value
  final MaterialColor Function(double value)? colorFunction;

  const ValueUnitWidget({
    Key? key,
    required this.value,
    required this.unit,
    this.colorFunction,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.end,
      crossAxisAlignment: CrossAxisAlignment.baseline,
      textBaseline: TextBaseline.alphabetic,
      children: [
        Text(
          value.toStringAsFixed(0),
          style: TextStyle(
            height: 1.0,
            fontWeight: FontWeight.bold,
            fontSize: 20,
            color: colorFunction != null
                ? colorFunction!(value)[500]!
                : Colors.grey.shade500,
          ),
        ),
        const SizedBox(width: 2),
        Text(
          unit,
          style: TextStyle(
              height: 1.0,
              fontSize: 16,
              color: colorFunction != null
                  ? colorFunction!(value)[600]!
                  : Colors.grey[600],
              fontWeight: FontWeight.bold),
        ),
      ],
    );
  }
}
