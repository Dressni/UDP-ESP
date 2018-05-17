import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

/**
 * 
 * @author Nick Klasse, welche bidirektionale Kommunikation zu ESP32 mittels UDP
 *         ermöglicht.
 * 
 */

public class UDPKommunikation implements Runnable {

	// Attribute
	private final int MAX_UDP_SIZE = 255;
	private DatagramSocket datagramSocket;
	private Scanner scanner;

	// Konstruktor
	public UDPKommunikation() throws Exception {
		datagramSocket = new DatagramSocket(6565);
		scanner = new Scanner(System.in);

		// Empfangsthread starten.
		Thread empfang = new Thread(this);
		empfang.start();

		// Ziel Adresse, sowie Port
		InetAddress zielIP = InetAddress.getByName("192.168.4.1");
		int zielPort = 1234;

		System.out.println("Geben sie etwas zu senden ein und bestätigen sie mit Eingabe.");

		// 1. Thread ist für die Eingabe des Benutzer verantwortlich.
		while (true) {
			// Eingegebenen Daten
			byte[] sendData = scanner.nextLine().trim().getBytes();

			// Senden der Daten
			DatagramPacket datagramPacket = new DatagramPacket(sendData, sendData.length, zielIP, zielPort);
			datagramSocket.send(datagramPacket);
		}
	}

	public static void main(String args[]) throws Exception {

		new UDPKommunikation();

	}

	@Override
	public void run() {
		// 2. Thread ist für das Empfangen der Daten vom ESP 32 verantwortlich.
		while (true) {
			// Halter für Empfangsdaten
			byte[] daten = new byte[MAX_UDP_SIZE];
			DatagramPacket empfangPaket = new DatagramPacket(daten, MAX_UDP_SIZE);

			// Es wird versucht ein Paket zu empfangen.
			try {
				datagramSocket.receive(empfangPaket);
			} catch (IOException e) {
				e.printStackTrace();
				continue;
			}
			// Ausgabe an Kommandozeile
			System.out.println("Empfang: " + new String(daten).trim());
		}
	}
}
