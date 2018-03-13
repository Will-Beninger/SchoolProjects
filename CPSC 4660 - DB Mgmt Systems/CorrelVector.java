//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: CorrelVector.java                                                  //
// Purpose: Vector to store all of the Correllogram data for an image       //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.awt.Color;
import java.util.Vector;
import java.lang.Math;

public class CorrelVector
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
  private CorrelPoint[] colorList;
  
  public CorrelVector(Picture pic)
  {
    discretizeImage(pic);
    createCorrelogram(pic);
  }
  
  public CorrelVector(CorrelPoint[] arrayOfColors)
  {
    colorList = new CorrelPoint[NUMCOLORS];
    System.arraycopy(arrayOfColors,0,colorList,0,NUMCOLORS);
  }
  
  private Picture discretizeImage(Picture pic)
  {
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
  
  private void createCorrelogram(Picture pic)
  {
    colorList = new CorrelPoint[NUMCOLORS];
    
    double[] kOne = new double[NUMCOLORS];
    double[] kThree = new double[NUMCOLORS];
    double[] kFive = new double[NUMCOLORS];
    double[] kSeven = new double[NUMCOLORS];
    int[] numPixelColors = new int[NUMCOLORS];
    
    for (int x=0; x<pic.width(); x++)
    {
      for (int y=0; y<pic.height(); y++)
      {
        Color pixelColor = pic.get(x,y);
        int colorIndex = findColorIndex(pixelColor);
        numPixelColors[colorIndex]++;
        //Search k1
        Vector<MyPoint> k1Neigh = findNeighbors(x,y,1,pic.width(),pic.height());
        if (k1Neigh.size() != 0)
        {
          kOne[colorIndex] = kOne[colorIndex] + (findNumColor(x,y,k1Neigh,pic)/(double)(k1Neigh.size()));
        }
        //Search k3
        Vector<MyPoint> k3Neigh = findNeighbors(x,y,3,pic.width(),pic.height());
        if (k3Neigh.size() != 0)
        {
          kThree[colorIndex] = kThree[colorIndex] + (findNumColor(x,y,k3Neigh,pic)/(double)(k3Neigh.size()));
        }
        //Search k5
        Vector<MyPoint> k5Neigh = findNeighbors(x,y,5,pic.width(),pic.height());
        if (k5Neigh.size() != 0)
        {
          kFive[colorIndex] = kFive[colorIndex] + (findNumColor(x,y,k5Neigh,pic)/(double)(k5Neigh.size()));
        }
        //Search k7
        Vector<MyPoint> k7Neigh = findNeighbors(x,y,7,pic.width(),pic.height());
        if (k7Neigh.size() != 0)
        {
          kSeven[colorIndex] = kSeven[colorIndex] + (findNumColor(x,y,k7Neigh,pic)/(double)(k7Neigh.size()));
        }
      }
    }
    
    //Average the pixels and put them in the vector
    for (int i=0; i<NUMCOLORS; i++)
    {
      if (numPixelColors[i] != 0)
      {
        kOne[i]=kOne[i]/numPixelColors[i];
        kThree[i]=kThree[i]/numPixelColors[i];
        kFive[i]=kFive[i]/numPixelColors[i];
        kSeven[i]=kSeven[i]/numPixelColors[i];
      }
      
      colorList[i] = new CorrelPoint(kOne[i],kThree[i],kFive[i],kSeven[i]);
    }
  }
  
  private int findColorIndex(Color labelColor)
  {
    for (int i=0; i < NUMCOLORS; i++)
    {
      if (labelColor.equals(colorArray[i]))
      {
        return i;
      }
    }
    return -1;
  }
  
  private Vector<MyPoint> findNeighbors(int x, int y, int dist, int maxx, int maxy)
  {
    Vector<MyPoint> returnPoints = new Vector<MyPoint>();
    //Search Bottom Horizontal
    for (int i=dist; i>=(0-dist); i--)
    {
      if (((x+i >= 0) && (y-dist >=0)) && ((x+i < maxx) && (y-dist < maxy)))
      {
        returnPoints.add(new MyPoint((x+i),(y-dist)));
      }
    }
    //Search Top Horizontal
    for (int i=dist; i>=(0-dist); i--)
    {
      if (((x+i >= 0) && (y+dist >=0)) && ((x+i < maxx) && (y+dist < maxy)))
      {
        returnPoints.add(new MyPoint((x+i),(y+dist)));
      }
    }
    //Search Left Vertical
    for (int i=(dist-1); i>=(1-dist); i--)
    {
      if (((x-dist >= 0) && (y+i >=0)) && ((x-dist < maxx) && (y+i < maxy)))
      {
        returnPoints.add(new MyPoint((x-dist),(y+i)));
      }
    }
    //Search Right Vertical
    for (int i=(dist-1); i>=(1-dist); i--)
    {
      if (((x+dist >= 0) && (y+i >=0)) && ((x+dist < maxx) && (y+i < maxy)))
      {
        returnPoints.add(new MyPoint((x+dist),(y+i)));
      }
    }
    return returnPoints;
  }
  
  private int findNumColor(int x,int y,Vector<MyPoint> neighList, Picture pic)
  {
    Color origColor = pic.get(x,y);
    int counter=0;
    for (int i=0; i<neighList.size(); i++)
    {
      if (origColor.equals((pic.get((int)(neighList.get(i).getx()), (int)(neighList.get(i).gety())))))
      {
        counter++;
      }
    }
    return counter;
  }
  
  public int size() {return colorList.length;}
  public CorrelPoint get(int i) {return colorList[i];}
  
  public double findChange(CorrelVector compV)
  {
    double result=0.0;
    
    //k=1
    for (int i=0; i<NUMCOLORS; i++)
    {
      double num = Math.abs(colorList[i].geta()-compV.get(i).geta());
      double denom = 1 + colorList[i].geta() + compV.get(i).geta();
      result = result + (num/denom);
    }
    
    //k=3
    for (int i=0; i<NUMCOLORS; i++)
    {
      double num = Math.abs(colorList[i].getb()-compV.get(i).getb());
      double denom = 1 + colorList[i].getb() + compV.get(i).getb();
      result = result + (num/denom);
    }
    //k=5
    for (int i=0; i<NUMCOLORS; i++)
    {
      double num = Math.abs(colorList[i].getc()-compV.get(i).getc());
      double denom = 1 + colorList[i].getc() + compV.get(i).getc();
      result = result + (num/denom);
    }
    //k=7
    for (int i=0; i<NUMCOLORS; i++)
    {
      double num = Math.abs(colorList[i].getd()-compV.get(i).getd());
      double denom = 1 + colorList[i].getd() + compV.get(i).getd();
      result = result + (num/denom);
    }
    
    return result;
  }
}