//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: FrontEnd.java                                                      //
// Purpose: Friendly front end interface for the image comparison           //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.util.Scanner;
import java.util.Vector;
import java.io.*;
import java.lang.Math;

public class FrontEnd
{
  public static void main(String[] args)
  {
    Vector<CCVdatabase> mainCCVDB = new Vector<CCVdatabase>();
    Vector<HistogramDatabase> mainHistDB = new Vector<HistogramDatabase>();
    Vector<CorrelDatabase> mainCorrelDB = new Vector<CorrelDatabase>();
    
    long histDBtime=0;
    long CCVDBtime = 0;
    long correlDBtime=0;
    
    long startTime=0;
    long endTime=0;
    double NANOTOSEC=1000000000.0;
    
    String wait;
    
    Scanner sc = new Scanner(System.in);
    System.out.println("Welcome to the image comparison program!");
    System.out.println("==========================================");
    
    //Menu
    boolean noSelection = true;
    String input = "";
    while (noSelection)
    {
      System.out.println("1) Create Database from images in ./pictureDatabase/ folder");
      System.out.println("2) Load Database from file");
      input = sc.nextLine();
      if (input.equals("1") || input.equals("2"))
      {
        noSelection=false;
      }
    }
    if (input.equals("1"))
    {
      System.out.println("System will now database all pictures in the \"pictureDatabase\" Directory.");
      File folder = new File("./pictureDatabase/");
      File[] listOfFiles = folder.listFiles();
      
      System.out.println("Would you like a print out of the current files in the directory? (Y/N)");
      input = "";
      while (!(input.equals("Y") || (input.equals("N"))))
      {
        input = sc.nextLine();
      }//Close Y/N decision
      
      if (input.equals("Y"))
      {
        for (int i=0; i < listOfFiles.length; i++)
        {
          System.out.println("File" + i + ": " + listOfFiles[i]);
        }//Close for loop of listing files
      }//Close if input.equals("Y")
      
      //Code to open each picture
      for (int i=0; i < listOfFiles.length; i++)
      {
        Picture tempHistPic = new Picture(listOfFiles[i]);
        tempHistPic.setOriginLowerLeft();
        startTime = System.nanoTime();
        HistogramVector tempPicHistVect = new HistogramVector(tempHistPic);
        mainHistDB.add(new HistogramDatabase(listOfFiles[i], tempPicHistVect));
        endTime = System.nanoTime();
        histDBtime += (endTime-startTime);
        
        Picture tempCCVPic = new Picture(listOfFiles[i]);
        tempCCVPic.setOriginLowerLeft();
        startTime = System.nanoTime();
        CCVvector tempPicVector = new CCVvector(tempCCVPic);
        mainCCVDB.add(new CCVdatabase(listOfFiles[i], tempPicVector));
        endTime = System.nanoTime();
        CCVDBtime += (endTime-startTime);
        
        Picture tempCorrelPic = new Picture(listOfFiles[i]);
        tempCorrelPic.setOriginLowerLeft();
        startTime = System.nanoTime();
        CorrelVector tempPicCorrelVect = new CorrelVector(tempCorrelPic);
        mainCorrelDB.add(new CorrelDatabase(listOfFiles[i], tempPicCorrelVect));
        endTime = System.nanoTime();
        correlDBtime += (endTime-startTime);
      }//Close for loop opening pictures
      
      System.out.println("To create the database from the images it took:");
      System.out.println("Histogram=" + histDBtime/NANOTOSEC + " seconds");
      System.out.println("CCV=" + CCVDBtime/NANOTOSEC + " seconds");
      System.out.println("Correlogram=" + correlDBtime/NANOTOSEC + " seconds");
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
      
      histDBtime=0;
      CCVDBtime = 0;
      correlDBtime=0;
      
      System.out.println("Now saving all Histogram database results into a flatfile titled: Histogram.txt");
      //Save the DB to a flatfile
      File myFile = null;
      FileWriter fout = null;
      try {
        myFile = new File("Histogram.txt");
        fout = new FileWriter(myFile, false);
        startTime = System.nanoTime();
        for (int i=0; i<mainHistDB.size(); i++)
        {
          String output = (mainHistDB.get(i).printDB());
          if (i!=mainHistDB.size()-1)
          {
            output += "\n";
          }
          fout.write(output);
        }
        endTime = System.nanoTime();
        histDBtime=endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
      try {
        fout.close();
      } catch (Exception ex) {}
      }
      
      System.out.println("Now saving all CCV database results into a flatfile titled: CCV.txt");
      
      //Save the DB to a flatfile
      myFile = null;
      fout = null;
      try {
        myFile = new File("CCV.txt");
        fout = new FileWriter(myFile, false);
        startTime = System.nanoTime();
        for (int i=0; i<mainCCVDB.size(); i++)
        {
          String output = (mainCCVDB.get(i).printDB());
          if (i!=mainCCVDB.size()-1)
          {
            output += "\n";
          }
          fout.write(output);
        }
        endTime = System.nanoTime();
        CCVDBtime = endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
      try {
        fout.close();
      } catch (Exception ex) {}
      }
      
      System.out.println("Now saving all Correl database results into a flatfile titled: Correlogram.txt");
      
      //Save the DB to a flatfile
      myFile = null;
      fout = null;
      try {
        myFile = new File("Correlogram.txt");
        fout = new FileWriter(myFile, false);
        startTime = System.nanoTime();
        for (int i=0; i<mainCorrelDB.size(); i++)
        {
          String output = (mainCorrelDB.get(i).printDB());
          if (i!=mainCorrelDB.size()-1)
          {
            output += "\n";
          }
          fout.write(output);
        }
        endTime = System.nanoTime();
        correlDBtime = endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
      try {
        fout.close();
      } catch (Exception ex) {}
      }
      
      System.out.println("To save the database flatfile it took:");
      System.out.println("Histogram=" + histDBtime/NANOTOSEC + " seconds");
      System.out.println("CCV=" + CCVDBtime/NANOTOSEC + " seconds");
      System.out.println("Correlogram=" + correlDBtime/NANOTOSEC + " seconds");
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
    }
    else if (input.equals("2"))
    {
      histDBtime=0;
      CCVDBtime = 0;
      correlDBtime=0;
      
      //Open DB from flatfile
      BufferedReader br = null;
      File histFile = new File("Histogram.txt");
      if (!(histFile.exists()))
      {
        System.out.println("Histogram.txt does not exist. Exiting");
        System.exit(-1);
      }
      try {
      br = new BufferedReader(new FileReader(histFile));
      String line;
      startTime = System.nanoTime();
      while ((line = br.readLine()) != null)
      {
        mainHistDB.add(new HistogramDatabase(line));
      }
      endTime = System.nanoTime();
      histDBtime=endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
        try {
      br.close();
        } catch (Exception ex) {}
      }
      
      br = null;
      File CCVfile = new File("CCV.txt");
      if (!(CCVfile.exists()))
      {
        System.out.println("CCV.txt does not exist. Exiting");
        System.exit(-1);
      }
      try {
      br = new BufferedReader(new FileReader(CCVfile));
      String line;
      startTime = System.nanoTime();
      while ((line = br.readLine()) != null)
      {
        mainCCVDB.add(new CCVdatabase(line));
      }
      endTime = System.nanoTime();
      CCVDBtime = endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
        try {
      br.close();
        } catch (Exception ex) {}
      }
      
      br = null;
      File Correlfile = new File("Correlogram.txt");
      if (!(Correlfile.exists()))
      {
        System.out.println("Correlogram.txt does not exist. Exiting");
        System.exit(-1);
      }
      try {
      br = new BufferedReader(new FileReader(Correlfile));
      String line;
      startTime = System.nanoTime();
      while ((line = br.readLine()) != null)
      {
        mainCorrelDB.add(new CorrelDatabase(line));
      }
      endTime = System.nanoTime();
      correlDBtime = endTime-startTime;
      } catch (IOException ex) {
        // report
      } finally {
        try {
      br.close();
        } catch (Exception ex) {}
      }
      
      System.out.println("To load from the database flatfile it took:");
      System.out.println("Histogram=" + histDBtime/NANOTOSEC + " seconds");
      System.out.println("CCV=" + CCVDBtime/NANOTOSEC + " seconds");
      System.out.println("Correlogram=" + correlDBtime/NANOTOSEC + " seconds");
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
    }
    boolean notDone=true;
    while (notDone)
    {
      histDBtime=0;
      CCVDBtime = 0;
      correlDBtime=0;
      
      System.out.println("Now beginning image comparison phase:");
      System.out.println("==========================================");
      System.out.println("Enter image number:");
      input="./pictureDatabase/img (";
      input+=sc.nextLine() + ").jpg";
      if(input.equals("./pictureDatabase/img ().jpg"))
      {
        System.out.println("No input received.");
        continue;
      }
      File compPicFile = new File(input);
      Picture comparePic = null;
      CCVvector compPicVector = null;
      HistogramVector compPicHist = null;
      CorrelVector compPicCorrel = null;
      MyPoint[] histChange = new MyPoint[mainHistDB.size()];
      MyPoint[] ccvChange = new MyPoint[mainHistDB.size()];
      MyPoint[] correlChange = new MyPoint[mainHistDB.size()];
      if (compPicFile.exists())
      {
        comparePic = new Picture(compPicFile);
        comparePic.setOriginLowerLeft();
        startTime = System.nanoTime();
        compPicHist = new HistogramVector(comparePic);
        endTime = System.nanoTime();
        histDBtime = endTime-startTime;
        
        comparePic = new Picture(compPicFile);
        comparePic.setOriginLowerLeft();
        startTime = System.nanoTime();
        compPicVector = new CCVvector(comparePic);
        endTime = System.nanoTime();
        CCVDBtime = endTime-startTime;
        
        comparePic = new Picture(compPicFile);
        comparePic.setOriginLowerLeft();
        startTime = System.nanoTime();
        compPicCorrel = new CorrelVector(comparePic);
        endTime = System.nanoTime();
        correlDBtime = endTime-startTime;
      }
      else
      {
        System.out.println("File does not exist!");
        continue;
      }
      System.out.println("The time to convert the comparison image to each format:");
      System.out.println("Histogram=" + histDBtime/NANOTOSEC + " seconds");
      System.out.println("CCV=" + CCVDBtime/NANOTOSEC + " seconds");
      System.out.println("Correlogram=" + correlDBtime/NANOTOSEC + " seconds");
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
      
      //Do things to compare the picture with the database.
      histDBtime=0;
      CCVDBtime = 0;
      correlDBtime=0;
      System.out.println("Now beginning Histogram Comparison");
      {//Organizing Hist
        File firstClosestFile = null;
        File secondClosestFile = null;
        File thirdClosestFile = null;
        double firstChange = 0.0;
        double secondChange = 0.0;
        double thirdChange = 0.0;
        startTime = System.nanoTime();
        for (int i=0; i<mainHistDB.size(); i++)
        {
          double change = compPicHist.findChange(mainHistDB.get(i).getHist());
          File dbFile = mainHistDB.get(i).getFilename();
          histChange[i] = new MyPoint(dbFile, change);
          //can comment out if you want to have it look for itself
          if (dbFile.equals(compPicFile))
          {
            continue;
          }
          if (firstClosestFile == null)
          {
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (change < firstChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = firstChange;
            secondClosestFile = firstClosestFile;
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (secondClosestFile == null)
          {
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if (change < secondChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if ((thirdClosestFile == null) || (change < thirdChange))
          {
            thirdChange = change;
            thirdClosestFile = dbFile;
          }
        }
        endTime = System.nanoTime();
        histDBtime = endTime - startTime;
        
        if (thirdClosestFile == null)
        {
          if (secondClosestFile == null)
          {
            if (firstClosestFile == null)
            {
              System.out.println("No files are in the comparison DB. Exiting");
              System.exit(-1);
            }
            else
            {
              secondClosestFile=firstClosestFile;
              thirdClosestFile=firstClosestFile;
            }
          }
          else
          {
            thirdClosestFile=secondClosestFile;
          }
        }
        
        System.out.print("The three closest files to \"" + compPicFile.toString() + "\" are:\n1) \"");
        System.out.print(firstClosestFile.toString() + "\" with change=" + firstChange + "\n2) \"");
        System.out.print(secondClosestFile.toString() + "\" with change=" + secondChange + "\n3) \"");
        System.out.println(thirdClosestFile.toString() + "\" with change=" + thirdChange + "");
        System.out.println("Now opening all 4 files...");
        comparePic = new Picture(compPicFile);
        comparePic.show();
        Picture firstClosestPicture = new Picture(firstClosestFile);
        firstClosestPicture.show();
        Picture secondClosestPicture = new Picture(secondClosestFile);
        secondClosestPicture.show();
        Picture thirdClosestPicture = new Picture(thirdClosestFile);
        thirdClosestPicture.show();
      }
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
      
      System.out.println("Now beginning CCV Comparison");
      {
        File firstClosestFile = null;
        File secondClosestFile = null;
        File thirdClosestFile = null;
        double firstChange = 0.0;
        double secondChange = 0.0;
        double thirdChange = 0.0;
        startTime = System.nanoTime();
        for (int i=0; i<mainCCVDB.size(); i++)
        {
          double change = compPicVector.findChange(mainCCVDB.get(i).getCCV());
          File dbFile = mainCCVDB.get(i).getFilename();
          ccvChange[i] = new MyPoint(dbFile, change);
          //can comment out if you want to have it look for itself
          if (dbFile.equals(compPicFile))
          {
            continue;
          }
          if (firstClosestFile == null)
          {
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (change < firstChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = firstChange;
            secondClosestFile = firstClosestFile;
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (secondClosestFile == null)
          {
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if (change < secondChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if ((thirdClosestFile == null) || (change < thirdChange))
          {
            thirdChange = change;
            thirdClosestFile = dbFile;
          }
        }
        endTime = System.nanoTime();
        CCVDBtime = endTime - startTime;
        
        if (thirdClosestFile == null)
        {
          if (secondClosestFile == null)
          {
            if (firstClosestFile == null)
            {
              System.out.println("No files are in the comparison DB. Exiting");
              System.exit(-1);
            }
            else
            {
              secondClosestFile=firstClosestFile;
              thirdClosestFile=firstClosestFile;
            }
          }
          else
          {
            thirdClosestFile=secondClosestFile;
          }
        }
        
        System.out.print("The three closest files to \"" + compPicFile.toString() + "\" are:\n1) \"");
        System.out.print(firstClosestFile.toString() + "\" with change=" + firstChange + "\n2) \"");
        System.out.print(secondClosestFile.toString() + "\" with change=" + secondChange + "\n3) \"");
        System.out.println(thirdClosestFile.toString() + "\" with change=" + thirdChange + "");
        System.out.println("Now opening all 4 files...");
        comparePic = new Picture(compPicFile);
        comparePic.show();
        Picture firstClosestPicture = new Picture(firstClosestFile);
        firstClosestPicture.show();
        Picture secondClosestPicture = new Picture(secondClosestFile);
        secondClosestPicture.show();
        Picture thirdClosestPicture = new Picture(thirdClosestFile);
        thirdClosestPicture.show();
      }
      
      System.out.println("Please hit enter when you want to continue");
      wait = sc.nextLine();
      
      System.out.println("Now beginning Correlogram Comparison");
      {
        File firstClosestFile = null;
        File secondClosestFile = null;
        File thirdClosestFile = null;
        double firstChange = 0.0;
        double secondChange = 0.0;
        double thirdChange = 0.0;
        startTime = System.nanoTime();
        for (int i=0; i<mainCorrelDB.size(); i++)
        {
          double change = compPicCorrel.findChange(mainCorrelDB.get(i).getCorrel());
          File dbFile = mainCorrelDB.get(i).getFilename();
          correlChange[i] = new MyPoint(dbFile, change);
          //can comment out if you want to have it look for itself
          if (dbFile.equals(compPicFile))
          {
            continue;
          }
          if (firstClosestFile == null)
          {
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (change < firstChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = firstChange;
            secondClosestFile = firstClosestFile;
            firstChange = change;
            firstClosestFile = dbFile;
          }
          else if (secondClosestFile == null)
          {
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if (change < secondChange)
          {
            thirdChange = secondChange;
            thirdClosestFile = secondClosestFile;
            secondChange = change;
            secondClosestFile = dbFile;
          }
          else if ((thirdClosestFile == null) || (change < thirdChange))
          {
            thirdChange = change;
            thirdClosestFile = dbFile;
          }
        }
        endTime = System.nanoTime();
        correlDBtime = endTime - startTime;
        
        if (thirdClosestFile == null)
        {
          if (secondClosestFile == null)
          {
            if (firstClosestFile == null)
            {
              System.out.println("No files are in the comparison DB. Exiting");
              System.exit(-1);
            }
            else
            {
              secondClosestFile=firstClosestFile;
              thirdClosestFile=firstClosestFile;
            }
          }
          else
          {
            thirdClosestFile=secondClosestFile;
          }
        }
        
        System.out.print("The three closest files to \"" + compPicFile.toString() + "\" are:\n1) \"");
        System.out.print(firstClosestFile.toString() + "\" with change=" + firstChange + "\n2) \"");
        System.out.print(secondClosestFile.toString() + "\" with change=" + secondChange + "\n3) \"");
        System.out.println(thirdClosestFile.toString() + "\" with change=" + thirdChange + "");
        System.out.println("Now opening all 4 files...");
        comparePic = new Picture(compPicFile);
        comparePic.show();
        Picture firstClosestPicture = new Picture(firstClosestFile);
        firstClosestPicture.show();
        Picture secondClosestPicture = new Picture(secondClosestFile);
        secondClosestPicture.show();
        Picture thirdClosestPicture = new Picture(thirdClosestFile);
        thirdClosestPicture.show();
      }
      
      System.out.println("The time to search each DB for similar images:");
      System.out.println("Histogram=" + histDBtime/NANOTOSEC + " seconds");
      System.out.println("CCV=" + CCVDBtime/NANOTOSEC + " seconds");
      System.out.println("Correlogram=" + correlDBtime/NANOTOSEC + " seconds");
      
      input="./pictureDatabase/img (";
      System.out.println("Enter image number for answer:");
      input+=sc.nextLine() + ").jpg";
      File answer = new File(input);
      if (!(answer.exists()))
        System.out.println("Invalid image number");
      else
      {
        //sort the point lists by the change
        System.out.println("The rank of the correct answer is:");
        int histRank = findRank(answer, histChange);
        int ccvRank = findRank(answer, ccvChange);
        int correlRank = findRank(answer, correlChange);
        System.out.println("Histogram=" + histRank);
        System.out.println("CCV=" + ccvRank);
        System.out.println("Correlogram=" + correlRank);
      }
      
      input="";
      while (!(input.equals("Y") || input.equals("N")))
      {
        System.out.println("Would you like to continue? (Y/N)");
        input=sc.nextLine();
        if(input.equals("Y"))
        {
          notDone = true;
        }
        else if (input.equals("N"))
        {
          notDone = false;
        }
      }
    }
  }//Close Main
  private static int findRank(File pNum, MyPoint[] a)
  {
    for (int i=a.length; i>0; i--)
    {
      for (int j=0; j<(i-1); j++)
      {
        if ((double)a[j].gety() > (double)a[j+1].gety())
        {
          MyPoint temp = a[j];
          a[j]=a[j+1];
          a[j+1]=temp;
        }
      }
    }
    
    for (int i=0; i<a.length; i++)
    {
      if (a[i].getx().equals(pNum))
      {
        return i;
      }
    }
    return -1;
  }
} //Close class FrontEnd