package timer;

import java.awt.Toolkit;
import java.time.Instant;

import javax.management.timer.Timer;
import javax.swing.JOptionPane;

import javafx.event.ActionEvent;

public class TimerTest {
  public static void main(String[] args) {
    TimePrinter listener = new TimePrinter();

    Timer timer = new Timer(1000, listener);
    timer.start();

    JOptionPane.showMessageDialog(null, "Quit program?");
    System.exit(0);
  }
}

class TimePrinter implements ActionListener {
  public void actionPerformed(ActionEvent event) {
    System.out.println("At the tone, the time is " +
                       Instant.ofEpochMilli(event.getWhen()));
    Toolkit.getDefaultToolkit().beep();
  }
}
