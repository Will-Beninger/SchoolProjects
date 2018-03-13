//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: CCVdatabase.java                                                   //
// Purpose: Database to store all of the CCVvectors                         //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.util.Vector;
import java.io.File;

public class CCVdatabase
{
  private File filename;
  private CCVvector fileCCV;
  private static int NUMCOLORS = 64;
  
  public CCVdatabase(File filename1, CCVvector fileCCV1)
  {
    filename=filename1;
    fileCCV = fileCCV1;
  }
  
  public CCVdatabase(String flatfile)
  {
    String firstDelims = "[~]";
    String[] firstTokens = flatfile.split(firstDelims);
    for (int j=0; j<firstTokens.length; j++)
    filename = new File(firstTokens[0]);
    
    String secondString = firstTokens[1];
    String secondDelims = "[,]";
    String[] secondTokens = secondString.split(secondDelims);
    
    MyPoint[] arrayOfColors = new MyPoint[NUMCOLORS];
    for(int i=0; i<NUMCOLORS; i++)
    {
    String thirdString = secondTokens[i];
    String thirdDelims = "[/]";
    String[] thirdTokens = thirdString.split(thirdDelims);
    arrayOfColors[i] = new MyPoint(Integer.parseInt(thirdTokens[0]), Integer.parseInt(thirdTokens[1]));
    }
    fileCCV = new CCVvector(arrayOfColors);
  }
  
  public File getFilename() {return filename;}
  public CCVvector getCCV() {return fileCCV;}
  
  public String printDB()
  {
    String output = filename.toString() + "~";
    output += printCCV();
    return output;
  }
  
  private String printCCV()
  {
    String output = "";
    
    for (int i=0; i<fileCCV.size(); i++)
    {
      output += fileCCV.get(i).getx() + "/" + fileCCV.get(i).gety();
      if (i!=(fileCCV.size()-1))
      {
        output += ",";
      }
    }
    return output;
  }
} //Close Class CCVdatabase