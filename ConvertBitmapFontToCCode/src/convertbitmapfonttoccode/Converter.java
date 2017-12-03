/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package convertbitmapfonttoccode;

import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

/**
 *
 * @author leo
 */
public class Converter {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        
        BufferedImage font = ImageIO.read(Converter.class.getResourceAsStream("/res/images.png"));
        
        System.out.print("    unsigned char font[128][128] = { \r\n");
        for (int y = 0; y < font.getHeight(); y++) {
            System.out.print("{ ");
            for (int x = 0; x < font.getWidth(); x++) {
                String color = font.getRGB(x, y) == -16777216 ? "0" : "1";
                System.out.print("" + color);
                if (x < font.getWidth() - 1) {
                    System.out.print("," + (x > 0 && (x % 13) == 0 ? "\r\n" : ""));
                }
            }
            System.out.print("}");
            if (y < font.getHeight() - 1) {
                System.out.print(",\r\n");
            }
            else {
                System.out.print("\r\n");
            }
        }
        System.out.print("};\r\n");
        
    }
    
}
