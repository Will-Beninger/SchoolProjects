//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: LabelColorCount.java                                               //
// Purpose: Quick structure to hold the labels and colors                   //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.awt.Color;
import java.util.Vector;

public class LabelColorCount
{
  //variables
  private int labelValue;
  private int count;
  private Color labelColor;
  
  public LabelColorCount(int value, Color tempColor)
  {
    labelValue=value;
    count=1;
    labelColor = tempColor;
  }
  
  public int getValue() {return labelValue;}
  public Color getColor() {return labelColor;}
  public int getCount() {return count;}
  
  public void addToCount() {count++;}
} //Close Class LabelColorCount