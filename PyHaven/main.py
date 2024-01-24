#! /usr/bin/env python3
# Character Encoding: UTF-8
#
# Here we provide the necessary imports.
# The basic GUI widgets are located in the QtWidgets module.
import sys
from PyQt5.QtWidgets import QApplication, QWidget

# Every PyQt5 application must create an application object.
# The application object is located in the QtWidgets module.
app = QApplication([])

# The QWidget widget is the base class of all user interface objects in PyQt5.
# We provide the default constructor for QWidget. The default constructor has no parent.
# A widget with no parent is called a window.
root = QWidget()

root.resize(320, 240)  # The resize() method resizes the widget.
root.setWindowTitle("Hello, World!")  # Here we set the title for our window.
root.show()  # The show() method displays the widget on the screen.

sys.exit(app.exec_())  # Finally, we enter the mainloop of the application.

# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


#def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
#    print(f'Hi, {name}')  # Press ⌘F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
#if __name__ == '__main__':
#    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
