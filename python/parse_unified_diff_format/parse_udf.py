#!/usr/bin/env python
import sys
import time

class codePatternC:   
   def __init__(self, start=0, size=0, code=""): # Constructor
      self.start     = start
      self.end       = start + size
      self.size      = size
      self.code      = code.split('\n')

   def getCodePattern(self):
      return self.code
   
   def getCodeStart(self):
      return self.start
   
   def getCodeEnd(self):
      return self.end

   def getCodeSize(self):
      return self.size

   def printCodePattern(self):
      line_count = self.start
      for line in self.code:
         print str(line_count) + "\t" + line
         line_count = line_count + 1


class singleDiffDataC():
   def __init__(self, minus_info, plus_info):
      self.precode   = codePatternC(int(minus_info[0]), int(minus_info[1]), minus_info[2])
      self.postcode  = codePatternC(int(plus_info[0]) , int(plus_info[1]) , plus_info[2])

   def printPreDiffCode(self):
      self.precode.printCodePattern()

   def printPostDiffCode(self):
      self.postcode.printCodePattern()

class multiDiffDataC():
   def __init__(self, start, end, diff_data):
      self.minus_name = None
      self.plus_name  = None
      self.start     = start
      self.end       = end
      self.diff_data = diff_data
      self.diff_code = []

   def readDiffCode(self, start, end, diff_data):
      minus_name, plus_name = "", ""
      minus_info, plus_info = [], []
      for index in range(start, end):
         if diff_data[index].startswith('---'):
            minus_name, minus_rev = self.getFileNameRevision(diff_data[index])
         elif diff_data[index].startswith('+++'):
            plus_name, plus_rev = self.getFileNameRevision(diff_data[index]) 
         elif diff_data[index].startswith('@@'):            
            minus_info, plus_info  = self.readCodePatternInfo(diff_data[index])
            index, minus_code, plus_code = self.readCodePattern(index+1, end, diff_data)
            minus_info.append(minus_code)
            plus_info.append(plus_code)
            if minus_name and plus_name:
               self.captureCodePattern(minus_name, plus_name, minus_info, plus_info)

   def captureCodePattern(self, minus_name, plus_name, minus_info, plus_info):
      self.minus_name = minus_name
      self.plus_name = plus_name
      self.diff_code.append(singleDiffDataC(minus_info, plus_info))

   def readCodePattern(self, start, end, diff_data):
      minus_code, plus_code  = "", ""
      for index in range(start, end):
         if diff_data[index].startswith('@@'): break
         elif diff_data[index].startswith('-'):
            minus_code = minus_code + diff_data[index][1:] + "\n"
         elif diff_data[index].startswith('+'):
            plus_code  = plus_code  + diff_data[index][1:] + "\n"
         else:
            minus_code = minus_code + diff_data[index] + "\n"
            plus_code  = plus_code  + diff_data[index] + "\n"
      return index, minus_code, plus_code

   def readCodePatternInfo(self, line):
      minus_info, plus_info  = [], []
      minus_start = minus_size = plus_start = plus_size = 0
      tokens = line.split(' ')
      for item in tokens:
         item = item.strip()
         if item.startswith('-'):
            minus_start, minus_size = self.getStartSize(item)
         elif item.startswith('+'):
            plus_start, plus_size   = self.getStartSize(item)

      minus_info.append(minus_start)            
      minus_info.append(minus_size)
      plus_info.append(plus_start)            
      plus_info.append(plus_size)            
      return minus_info, plus_info

   def getFileNameRevision(self, line):
      tuples= line.partition('\t')
      name  = tuples[0].strip().partition(' ')[2]
      rev   = tuples[2].strip().partition(' ')[2].partition(')')[0]
      return name, rev

   def getStartSize(self, item):
      tp = item[1:].partition(',')
      start = int(tp[0].strip())
      size  = int(tp[2].strip())
      return start, size

   def printDiffData(self, start, end, diff_data):
      print "\n\nFile Diff Data : "
      for index in range(start, end):
         print str(index) + "\t" + diff_data[index].strip()

   def printMultiDiffCode(self):
      self.printDiffData(self.start, self.end, self.diff_data)
      print "\n\nFile Name : " + self.minus_name
      print "PreCode :"
      for obj in self.diff_code:
         obj.printPreDiffCode()
      print "File Name : " + self.plus_name
      print "PostCode :"
      for obj in self.diff_code:
         obj.printPostDiffCode()

class udfC():
   def __init__(self):
      self.diff_file = []

   def isStartOfDiffCode(self, line):
      return line.startswith('Index:')

   def isEndofDiffCode(self, index, diff_data):
      return index >= len(diff_data)

   def parseUDF(self, diff_data):
      index = 0
      start = 0
      end   = 0
      while True:
         if self.isEndofDiffCode(index, diff_data): break
         if self.isStartOfDiffCode(diff_data[index]):
            start = index
            while True:
               index = index + 1                                              
               if self.isEndofDiffCode(index, diff_data):   break
               if self.isStartOfDiffCode(diff_data[index]): break
            end   = index
            obj   = multiDiffDataC(start, end, diff_data)                  
            obj.readDiffCode(start, end, diff_data)
            self.diff_file.append(obj)                                                
         index = index + 1
            
   def printParsedUDF(self):            
      for obj in self.diff_file:
         obj.printMultiDiffCode()

if __name__ == "__main__":
   if len(sys.argv) < 2:
      print "Supply diff format filename"
      sys.exit(1)
   else:
      start    = time.time()
      filename = sys.argv[1] 
      try:
         fp = open(filename)
         diff_data_list = fp.readlines()
         fp.close()
      except IOError:
         print "Error: Failed to open file " + filename
         sys.exit(1)
      udf_obj  = udfC()
      udf_obj.parseUDF(diff_data_list)
      udf_obj.printParsedUDF()
      end = time.time()
      print "Time to complete : " + repr(end-start)

