# coding=utf-8
import os
import sys
import codecs

def removeBom(file):
    BOM = b'\xef\xbb\xbf'
    existBom = lambda s: True if s==BOM else False
 
    f = open(file, 'rb')
    if existBom( f.read(3) ):
        fbody = f.read()
        #f.close()
        with open(file, 'wb') as f:
            f.write(fbody)
 
fileList = []
for dirpath, dirnames, filenames in os.walk("."):
    for filename in filenames:
        #print(filename)
        if os.path.splitext(filename)[1] == '.vcxproj':
            filepath = os.path.join(dirpath, filename)
            #print("file:" + filepath)
            fileList.append(filepath)
print(fileList)

for i in range(len(fileList)):
    #traverse file list
    fileName = fileList[i]
    print(fileName)
    removeBom(fileName)   
    vcFile = open(fileName, encoding="utf-8")
    
    fileContent = ""
    Count = 1
    while True:
        textLines = vcFile.readline()
        if not textLines:
            break;
        #print(textLines)
        
    # debug infomation
        debugStr = "</PreprocessorDefinitions>"
        debugPos = textLines.find(debugStr)
    #add debug include info    
        if (debugPos != -1):
            debugPos += len(debugStr)
            addIncludeContent= "\n      <AdditionalIncludeDirectories>$(OSG_ROOT)\include;</AdditionalIncludeDirectories>"
            textLines = textLines[:debugPos] + addIncludeContent + textLines[debugPos:]    
            
            nextText = vcFile.readline()
            if nextText.find("AdditionalIncludeDirectories") == -1:                   
                fileContent += textLines;     
                fileContent += nextText;
                print("exceed to adding include")
                continue;
            else:
                fileContent += textLines
                print("exceed to adding include")				
                continue;
                
    #add debug lib and input info
        libStr = "<GenerateDebugInformation>true</GenerateDebugInformation>"
        libPos = textLines.find(libStr);
        
        if (libPos != -1):
            libPos += len(libStr)
            addLibContent = "\n      <AdditionalLibraryDirectories>$(OSG_ROOT)/lib2012;</AdditionalLibraryDirectories>\n      <AdditionalDependencies>osgd.lib;osgDBd.lib;osgViewerd.lib;osgUtild.lib;osgGAd.lib;osgTextd.lib</AdditionalDependencies>"
            addLibContentRea = "\n      <AdditionalLibraryDirectories>$(OSG_ROOT)/lib2012;</AdditionalLibraryDirectories>\n      <AdditionalDependencies>osg.lib;osgDB.lib;osgViewer.lib;osgUtil.lib;osgGA.lib;osgText.lib</AdditionalDependencies>"
            if Count == 1:
                textLines = textLines[:libPos] + addLibContent + textLines[libPos:] 
            else:
                textLines = textLines[:libPos] + addLibContentRea + textLines[libPos:] 
            
            nextLibText = vcFile.readline()
            nextInputText = vcFile.readline()                   
            print(nextLibText)
            print(nextInputText)
                   
            if nextLibText.find("AdditionalLibraryDirectories") != -1:
                if nextInputText.find("AdditionalDependencies") == -1:
                    fileContent += textLines
                    fileContent += nextInputText
                elif nextInputText.find("AdditionalDependencies") != -1:
                    fileContent += textLines        
            else:
                if nextLibText.find("AdditionalDependencies") != -1:
                    fileContent += textLines
                    fileContent += nextInputText
                elif nextInputText.find("AdditionalDependencies") == -1:
                    fileContent += textLines
                    fileContent += nextLibText
                    fileContent += nextInputText                    
                  
            print("exceed to add lib and input")
            Count += 1
            continue;
             
        fileContent += textLines;

    newFile = open(fileName, "w+", encoding="utf-8")
    newFile.write(fileContent)  
    newFile.close()    
    vcFile.close;

os.system("pause")