import 'dart:io';

void main() {
  stdout.write('Enter your name: ');
  String? name = stdin.readLineSync(); // Read user input

  stdout.write('Enter your age: ');
  int age = int.parse(stdin.readLineSync()!); // Convert string to int

  print('Hello, $name! You are $age years old.');
}
