#include "Adafruit_DHT.h"

#define DHTPIN 0     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11		// DHT 11 
#define DHTTYPE DHT22		// DHT 22 (AM2302)
//#define DHTTYPE DHT21		// DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Note by WDC: This is a pull-up from data to VCC so a 4.7k should work too

DHT dht(DHTPIN, DHTTYPE);

// Variables
double temp_k;
double temp_c;
double temp_f;
double humidity;
double dew_point;
double heat_index;
String my_status;

// API key for channel in ThingSpeak
const String key = "F4EPVGFZZ9C0JRM2";

// Seconds
const int second = 1000;
const int minute = 60*second;
const int hour = 60*minute;

void setup()
{
    // Initialize sensor
    dht.begin();
    my_status = "Initializing...";
    
    // Initialize variables
    Particle.variable("temp_k", &temp_k, DOUBLE);
    Particle.variable("temp_c", &temp_c, DOUBLE);
    Particle.variable("temp_f", &temp_f, DOUBLE);
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("dew_point", &dew_point, DOUBLE);
    Particle.variable("heat_index", &heat_index, DOUBLE);
    Particle.variable("status", &my_status, STRING);
}

void loop()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a 
    // very slow sensor)
	humidity = dht.getHumidity();
    // Read temperature as Celsius
	temp_c = dht.getTempCelcius();
    // Read temperature as Farenheit
	temp_f = dht.getTempFarenheit();
  
    // Check if any reads failed and exit early (to try again).
	if (isnan(humidity) || isnan(temp_c) || isnan(temp_f)) {
	    
        // Send error message
        my_status = "Warning: Failed to read sensor.";
		Particle.publish("DHT22_status",my_status,PRIVATE);

    } else {

        // Compute dew point and heat index
        dew_point = dht.getDewPoint();
        heat_index = dht.getHeatIndex();
        temp_k = dht.getTempKelvin();
    
        // Publish status
        my_status = "OK.";
		Particle.publish("DHT22_status",my_status,PRIVATE);

        // Publish temperature and humidity
        String DHT22_data = String("{ ") +
            "\"1\": \"" + String(temp_k) + "\"," +
            "\"2\": \"" + String(temp_c) + "\"," +
            "\"3\": \"" + String(temp_f) + "\"," +
            "\"4\": \"" + String(humidity) + "\"," +
            "\"5\": \"" + String(dew_point) + "\"," +
            "\"6\": \"" + String(heat_index) + "\"," +
            "\"k\": \"" + key + "\" }";
        Particle.publish("DHT22_data",DHT22_data,PRIVATE);
    }

    delay(2*minute);
}

