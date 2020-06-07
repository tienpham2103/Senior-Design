import java.awt.Color;
import java.util.Scanner;

public class DepthToColor
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);
        boolean cont = true;
        
        final int MAX_DEPTH = 60; // Max depth of 1000
        final int MIN_DEPTH = 0;
        final int MAX_HSL = 240; // I chose 240 because that is dark blue
        final int MIN_HSL = 0;
        
        String ans, hex;
        int rgb, red, blue, green;
        float hue, sat, lit, ratio, depth;
        
        while (cont)
        {
            System.out.print("Enter depth or type \"stop\": ");
            ans = scanner.nextLine();
            outer:
            if (isNumeric(ans))
            {
                depth = Float.parseFloat(ans);

                if (depth > MAX_DEPTH) // if depth is over max, it is registered as the max
                {
                    depth = MAX_DEPTH;
                }
                
                ratio = (depth - MIN_DEPTH) / (MAX_DEPTH - MIN_DEPTH);

                hue = (ratio * (MAX_HSL - MIN_HSL) + MIN_HSL) / 360;
                sat = 1;
                lit = 1;

                rgb = Color.HSBtoRGB(hue, sat, lit);
                red = (rgb>>16)&0xFF;
                green = (rgb>>8)&0xFF;
                blue = rgb&0xFF;
                
                hue *= 360; // these are initially stored as decimals for conversion
                sat *= 100;
                lit *= 100;
                
                hex = String.format("#%02x%02x%02x", red, green, blue);
                
                System.out.printf("---------------\nHSL: %.0f,%.0f,%.0f\nRGB: %d, %d, %d\nHex: %s\n---------------\n", hue, sat, lit, red, green, blue, hex);
            }
            else if(ans.equals("stop"))
            {
                cont = false;
            }
            else
            {
                System.out.print("\nThe depth you typed was not a number. Try again \n");
            }
        }
    }
    
    public static boolean isNumeric(String str) // checks if the string is a number or not
    {
        try
        {
            Float.parseFloat(str);
            return true;
        } catch(NumberFormatException e)
        {
            return false;
        }
    }
}
