using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Net;

namespace ArduinoToWindowsToEnergyMonitor
{
    class Program
    {
        /// <summary>Copy here your read and write API key from open energy monitor</summary>
        /// <seealso cref="http://emoncms.org/input/api"/>
        const string kOpenEnergyMonitorApiKey = "db255478f4103...";

        static void Main(string[] args)
        {
            // open serial port communications with your arduino board
            // you may need to change COM3 to some other port number
            // check in device manager to see which port is used by arduino
            SerialPort serialPort = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);

            serialPort.Open();

            while(true)
            {
                string message = null;

                try
                {
                    // receive a line of text from arduino
                    message = serialPort.ReadLine();

                    // remove newline used to separate messages coming from arduino
                    message = message.Trim();

                    if (string.IsNullOrWhiteSpace(message) == false)
                    {
                        // message should be something like: light|28|power|651|temperature|22
                        // we split it here in six parts separated by the | character
                        string[] parts = message.Split('|');

                        if (parts.Length == 6)
                        {
                            // part integer readings for light, power and temperature
                            int lightValue = int.Parse(parts[1]);
                            int powerValue = int.Parse(parts[3]);
                            int tempValue = int.Parse(parts[5]);

                            // url that we need to call to pass our readings to open energy
                            string eneryMonitorUrl = "http://emoncms.org/input/post.json?json={light:" + lightValue + ",power:" + powerValue + ",temperature:" + tempValue + "}&apikey=" + kOpenEnergyMonitorApiKey;

                            // we do an HTTP GET to open energy monitor
                            WebClient webClient = new WebClient();
                            string energyMonitorResponse = webClient.DownloadString(new Uri(eneryMonitorUrl));

                            Console.WriteLine("arduino says: " + message);
                            Console.WriteLine("we call: " + eneryMonitorUrl);
                            Console.WriteLine("energy monitor responds: " + energyMonitorResponse);
                            Console.WriteLine(Environment.NewLine);
                        }
                    }
                }

                catch (Exception exception)
                {
                    Console.WriteLine("An error occoured, message received: '" + message + "', exception: " + exception);
                }
            }
        }
    }
}

