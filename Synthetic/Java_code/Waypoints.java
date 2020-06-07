import java.lang.Math;
import java.util.ArrayList;

public class Waypoints
{
    
    private double decimal_lat, decimal_long;
    private double NMEA_lat,    NMEA_long;
    private int depth;
    private char char_lat, char_long;
    
    Waypoints()
    {

    }

    Waypoints(double lattitude, double longitude)
    {
        this.decimal_lat = lattitude;
        this.decimal_long = longitude;
    }

    void setDepth(int depth)
    { this.depth = depth; }

    void setLattitude(double decimal_lat) //setter
    { this.decimal_lat = decimal_lat; }

    void setLongitude(double decimal_long) //setter
    { this.decimal_long = decimal_long; }

    int getDepth()
    {return depth;}

    double getLattitude() //getter
    {return decimal_lat; }

    double getLongitude() //getter
    { return decimal_long; }

    void convertToNMEA() //converts decimal to NMEA
    {
        double lat, lon;
        lat = Math.abs(this.decimal_lat);
        lon = Math.abs(this.decimal_long);
        
        double temp = lat - Math.floor(lat);
        this.NMEA_lat = (Math.floor(lat) * 100) + (temp * 60);
        
        temp = lon - Math.floor(lon);
        this.NMEA_long = (Math.floor(lon) * 100) + (temp * 60);
    }
    
    void displayNMEA() // displays in console the NMEA coordinates
    {
        convertToNMEA();
        char lat, lon;
        if (this.decimal_lat > 0)
            lat = 'N';
        else
            lat = 'S';
        if (this.decimal_long > 0)
            lon = 'E';
        else
            lon = 'W';
        
        System.out.printf("The NMEA coordinates for this waypoint are %f%c %f%c\n", this.NMEA_lat, lat, this.NMEA_long, lon);
    }
}
