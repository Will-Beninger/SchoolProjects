//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: CCVvector.java                                                     //
// Purpose: Vector to store CCVpairs                                        //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.awt.Color;
import java.util.Vector;
import java.lang.Math;

public class CCVvector
{
  //variables
  private MyPoint[] colorList;
  private int mincoherent;
  
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
  
  //Numbers
  private static double GAMMACORRECT = 2.2;
  private static int NUMCOLORS = 64;
  
  public CCVvector(Picture pic)
  {
    mincoherent = (int)((pic.height()*pic.width())*0.01);
    pic = blurImage(pic);
    pic = discretizeImage(pic);
    computeCCV(pic);
  }
  
  public CCVvector(MyPoint[] arrayOfColors)
  {
    colorList = new MyPoint[NUMCOLORS];
    System.arraycopy(arrayOfColors,0,colorList,0,NUMCOLORS);
    mincoherent=0;
  }
  
  private Picture blurImage(Picture pic)
  {
    int picHeight = pic.height();
    int picWidth = pic.width();
    
    for (int x=0; x<picWidth; x++)
    {
      for (int y=0; y<picHeight; y++)
      {
        int totalRed=0;
        int totalGreen=0;
        int totalBlue=0;
        Vector<MyPoint> adjPixels = findAdjoiningPixels(x, y, picWidth, picHeight);
        for (int i=0; i<adjPixels.size(); i++)
        {
          Color tempPointColor = pic.get((int)adjPixels.get(i).getx(), (int)adjPixels.get(i).gety());
          totalRed += tempPointColor.getRed();
          totalGreen += tempPointColor.getGreen();
          totalBlue += tempPointColor.getBlue();
        }
        int blurRed = totalRed / adjPixels.size();
        int blurGreen = totalGreen / adjPixels.size();
        int blurBlue = totalBlue / adjPixels.size();
        Color blurColor = new Color(blurRed, blurGreen, blurBlue);
        
        for (int i=0; i<adjPixels.size(); i++)
        {
          pic.set((int)adjPixels.get(i).getx(), (int)adjPixels.get(i).gety(), blurColor);
        }
      }
    }
    return pic;
  }
  
  private Vector<MyPoint> findAdjoiningPixels(int x, int y, int maxX, int maxY)
  {
    Vector<MyPoint> adjPixels = new Vector<MyPoint>();
    //Need to check a total of eight surrounding pixels
    for (int i=-1; i<2; i++) //x axis
    {
      for (int j=-1; j<2; j++)
      {
        if (((x+i >= 0) && (y+j >= 0)) && ((x+i < maxX) && (y+j < maxY)))
        {
          adjPixels.add(new MyPoint(x+i, y+j));
        }
      }
    }
    return adjPixels;
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
  
  private void computeCCV(Picture pic)
  {
    colorList = new MyPoint[NUMCOLORS];
    for (int i=0; i<NUMCOLORS; i++)
    {
      colorList[i] = new MyPoint();
    }
    
    int[][] picValues = new int[pic.width()][pic.height()];
    int nextLabel=0;
    Vector<Vector<Integer>> equivLabels = new Vector<Vector<Integer>>();
    
    //first pass
    for (int x=0; x<pic.width(); x++)
    {
      for (int y=0; y<pic.height(); y++)
      {
        Vector<MyPoint> neighborList = findNeighbors(pic, x, y);
        if (neighborList.isEmpty())
        {
          Vector<Integer> tempLabel = new Vector<Integer>();
          tempLabel.add(nextLabel);
          equivLabels.add(tempLabel);
          picValues[x][y] = nextLabel;
          nextLabel++;
        }
        else
        {
          Vector<Integer> neighLabelList = new Vector<Integer>();
          for (int i=0; i < neighborList.size(); i++)
          {
            int tempNeighLabel = picValues[(int)neighborList.get(i).getx()][(int)neighborList.get(i).gety()];
            if (!(neighLabelList.contains(tempNeighLabel)))
            {
              neighLabelList.add(tempNeighLabel);
            }
          }
          int minLabel = neighLabelList.get(0);
          for (int i=1; i < neighLabelList.size(); i++)
          {
            if (neighLabelList.get(i) < minLabel)
            {
              minLabel = neighLabelList.get(i);
            }
          }
          picValues[x][y] = minLabel;
          for (int i=0; i < neighLabelList.size(); i++)
          {
            if (!(equivLabels.get(minLabel).contains(neighLabelList.get(i))))
            {
              equivLabels.get(minLabel).add(neighLabelList.get(i));
            }
          }
        }
      }
    }
    //End First Pass
    
    //Second Pass
    for (int x=0; x<pic.width(); x++)
    {
      for (int y=0; y<pic.height(); y++)
      {
        int minLabel = picValues[x][y];
        for (int i=0; i < equivLabels.get(picValues[x][y]).size(); i++)
        {
          if (equivLabels.get(picValues[x][y]).get(i) < minLabel)
          {
            minLabel = equivLabels.get(picValues[x][y]).get(i);
          }
        }
        picValues[x][y] = minLabel;
      }
    }
    
    //Need to find the number of contiguous colors that meet threshold
    //Third Pass
    //Will be counting the number of pixels with value of x and assigning it a color based on that.
    Vector<LabelColorCount> componentsTable = new Vector<LabelColorCount>();
    for (int x=0; x<pic.width(); x++)
    {
      for (int y=0; y<pic.height(); y++)
      {
        int tempLocation = searchForLabel(picValues[x][y], componentsTable);
        if (tempLocation == -1) //not found
        {
          componentsTable.add(new LabelColorCount(picValues[x][y], pic.get(x,y)));
        }
        else
        {
          componentsTable.get(tempLocation).addToCount();
        }
      }
    }
    
    //Need to tally all the contiguous and non-contiguous color regions
    for (int i=0; i < componentsTable.size(); i++)
    {
      //figure out which color to put it in
      Color labelColor = componentsTable.get(i).getColor();
      int position = findColorIndex(labelColor);
      if (position == -1)
      {
        System.out.println("CRITICAL ERROR. Color not found");
        System.exit(-1);
      }
      int tempCount = componentsTable.get(i).getCount();
      if (tempCount > mincoherent)
      {
        colorList[position].setx(((int)colorList[position].getx() + tempCount));
      }
      else
      {
        colorList[position].sety(((int)colorList[position].gety() + tempCount));
      }
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
  
  private int searchForLabel(int labelValue, Vector<LabelColorCount> table)
  {
    for (int i=0; i < table.size(); i++)
    {
      if (table.get(i).getValue() == labelValue)
      {
        return i;
      }
    }
    return -1;
  }
  
  private Vector<MyPoint> findNeighbors(Picture pic, int x, int y)
  {
    Vector<MyPoint> neighborList = new Vector<MyPoint>();
    Color mainPoint = pic.get(x,y);
    Color compPoint;
    //Check NW is in range
    if (((x-1 >= 0) && (y+1 >= 0)) && ((x-1 < pic.width()) && (y+1 < pic.height())))
    {
      compPoint = pic.get(x-1, y+1);
      if (mainPoint.equals(compPoint))
      {
        neighborList.add(new MyPoint(x-1, y+1));
      }
    }
    //Check W is in range
    if (((x-1 >= 0) && (y >= 0)) && ((x-1 < pic.width()) && (y < pic.height())))
    {
      compPoint = pic.get(x-1, y);
      if (mainPoint.equals(compPoint))
      {
        neighborList.add(new MyPoint(x-1, y));
      }
    }
    //Check SW is in range
    if (((x-1 >= 0) && (y-1 >= 0)) && ((x-1 < pic.width()) && (y-1 < pic.height())))
    {
      compPoint = pic.get(x-1, y-1);
      if (mainPoint.equals(compPoint))
      {
        neighborList.add(new MyPoint(x-1, y-1));
      }
    }
    //Check S is in range
    if (((x >= 0) && (y-1 >= 0)) && ((x < pic.width()) && (y-1 < pic.height())))
    {
      compPoint = pic.get(x, y-1);
      if (mainPoint.equals(compPoint))
      {
        neighborList.add(new MyPoint(x, y-1));
      }
    }
    return neighborList;
  }
  
  public int size()
  {
    return colorList.length;
  }
  
  public MyPoint get(int i)
  {
    return colorList[i];
  }
  
  public double findChange(CCVvector compareV)
  {
    double result = 0.0;
    for (int i=0; i < NUMCOLORS; i++)
    {
      result += Math.abs(((int)colorList[i].getx()-(int)compareV.get(i).getx()));
      result += Math.abs(((int)colorList[i].gety()-(int)compareV.get(i).gety()));
    }
    return result;
  }
  
} //Close Class CCVvector