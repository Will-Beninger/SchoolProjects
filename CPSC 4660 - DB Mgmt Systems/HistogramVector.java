//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: HistogramVector.java                                               //
// Purpose: Vector to store all of the Histogram data for an image          //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.awt.Color;

public class HistogramVector
{
  //CONSTANTS
  //Numbers:
  private static double GAMMACORRECT = 2.2;
  private static int NUMCOLORS = 64;
  
    //CONSTANTS
  private static Color COLOR0 = new Color(31,31,31);
  private static Color COLOR1 = new Color(31,31,95);
  private static Color COLOR2 = new Color(31,31,159);
  private static Color COLOR3 = new Color(31,31,223);
  private static Color COLOR4 = new Color(31,95,31);
  private static Color COLOR5 = new Color(31,95,95);
  private static Color COLOR6 = new Color(31,95,159);
  private static Color COLOR7 = new Color(31,95,223);
  private static Color COLOR8 = new Color(31,159,31);
  private static Color COLOR9 = new Color(31,159,95);
  private static Color COLOR10 = new Color(31,159,159);
  private static Color COLOR11 = new Color(31,159,223);
  private static Color COLOR12 = new Color(31,223,31);
  private static Color COLOR13 = new Color(31,223,95);
  private static Color COLOR14 = new Color(31,223,159);
  private static Color COLOR15 = new Color(31,223,223);
  private static Color COLOR16 = new Color(95,31,31);
  private static Color COLOR17 = new Color(95,31,95);
  private static Color COLOR18 = new Color(95,31,159);
  private static Color COLOR19 = new Color(95,31,223);
  private static Color COLOR20 = new Color(95,95,31);
  private static Color COLOR21 = new Color(95,95,95);
  private static Color COLOR22 = new Color(95,95,159);
  private static Color COLOR23 = new Color(95,95,223);
  private static Color COLOR24 = new Color(95,159,31);
  private static Color COLOR25 = new Color(95,159,95);
  private static Color COLOR26 = new Color(95,159,159);
  private static Color COLOR27 = new Color(95,159,223);
  private static Color COLOR28 = new Color(95,223,31);
  private static Color COLOR29 = new Color(95,223,95);
  private static Color COLOR30 = new Color(95,223,159);
  private static Color COLOR31 = new Color(95,223,223);
  private static Color COLOR32 = new Color(159,31,31);
  private static Color COLOR33 = new Color(159,31,95);
  private static Color COLOR34 = new Color(159,31,159);
  private static Color COLOR35 = new Color(159,31,223);
  private static Color COLOR36 = new Color(159,95,31);
  private static Color COLOR37 = new Color(159,95,95);
  private static Color COLOR38 = new Color(159,95,159);
  private static Color COLOR39 = new Color(159,95,223);
  private static Color COLOR40 = new Color(159,159,31);
  private static Color COLOR41 = new Color(159,159,95);
  private static Color COLOR42 = new Color(159,159,159);
  private static Color COLOR43 = new Color(159,159,223);
  private static Color COLOR44 = new Color(159,223,31);
  private static Color COLOR45 = new Color(159,223,95);
  private static Color COLOR46 = new Color(159,223,159);
  private static Color COLOR47 = new Color(159,223,223);
  private static Color COLOR48 = new Color(223,31,31);
  private static Color COLOR49 = new Color(223,31,95);
  private static Color COLOR50 = new Color(223,31,159);
  private static Color COLOR51 = new Color(223,31,223);
  private static Color COLOR52 = new Color(223,95,31);
  private static Color COLOR53 = new Color(223,95,95);
  private static Color COLOR54 = new Color(223,95,159);
  private static Color COLOR55 = new Color(223,95,223);
  private static Color COLOR56 = new Color(223,159,31);
  private static Color COLOR57 = new Color(223,159,95);
  private static Color COLOR58 = new Color(223,159,159);
  private static Color COLOR59 = new Color(223,159,223);
  private static Color COLOR60 = new Color(223,223,31);
  private static Color COLOR61 = new Color(223,223,95);
  private static Color COLOR62 = new Color(223,223,159);
  private static Color COLOR63 = new Color(223,223,223);
  
  
  //Array of all the colors
  Color[] colorArray = {COLOR0,COLOR1,COLOR2,COLOR3,COLOR4,
    COLOR5,COLOR6,COLOR7,COLOR8,COLOR9,COLOR10,COLOR11,
    COLOR12,COLOR13,COLOR14,COLOR15,COLOR16,COLOR17,
    COLOR18,COLOR19,COLOR20,COLOR21,COLOR22,COLOR23,
    COLOR24,COLOR25,COLOR26,COLOR27,COLOR28,COLOR29,
    COLOR30,COLOR31,COLOR32,COLOR33,COLOR34,COLOR35,
    COLOR36,COLOR37,COLOR38,COLOR39,COLOR40,COLOR41,
    COLOR42,COLOR43,COLOR44,COLOR45,COLOR46,COLOR47,
    COLOR48,COLOR49,COLOR50,COLOR51,COLOR52,COLOR53,
    COLOR54,COLOR55,COLOR56,COLOR57,COLOR58,COLOR59,
    COLOR60,COLOR61,COLOR62,COLOR63};
  
  //Variables:
  private int[] colorList;
  
  public HistogramVector(Picture pic)
  {
    discretizeImage(pic);
  }
  
  public HistogramVector(int[] arrayOfColors)
  {
    colorList = new int[NUMCOLORS];
    System.arraycopy(arrayOfColors,0,colorList,0,NUMCOLORS);
  }
  
  private Picture discretizeImage(Picture pic)
  {
    colorList = new int[NUMCOLORS];
    
    for (int x=0; x < pic.width(); x++)
    {
      for (int y=0; y < pic.height(); y++)
      {
        double minDistColor=0;
        int minColor=0;
        for (int i=0; i<NUMCOLORS; i++)
        {
          double tempDistance = eucColorDistance(pic.get(x,y), colorArray[i]);
          if (i == 0)
          {
            minDistColor=tempDistance;
            minColor=i;
          }
          if  (tempDistance < minDistColor)
          {
            minDistColor=tempDistance;
            minColor=i;
          }
        }
        pic.set(x,y,colorArray[minColor]);
        colorList[minColor]+=1;
      }
    }
    return pic;
  }
  
  private double eucColorDistance(Color c1, Color c2)
  {
    return Math.sqrt(
                     Math.pow((3*(corrRGB(c1.getRed())-corrRGB(c2.getRed()))), 2) 
                     + Math.pow((4*(corrRGB(c1.getGreen())-corrRGB(c2.getGreen()))), 2)
                     + Math.pow((2*(corrRGB(c1.getBlue())-corrRGB(c2.getBlue()))), 2)
                    );
  }
  
  private double corrRGB (int x)
  {
    return (255.0 * (Math.pow((x/255.0), 2.2)));
  }
  
  public int size()
  {
    return colorList.length;
  }
  
  public int get(int i)
  {
    return colorList[i];
  }
  
  public double findChange(HistogramVector compareV)
  {
    double result = 0.0;
    for (int i=0; i < NUMCOLORS; i++)
    {
      result += Math.abs((colorList[i]-compareV.get(i)));
    }
    return result;
  }
}