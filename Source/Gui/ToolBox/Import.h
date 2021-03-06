#pragma once
#ifndef DSO_GUI_TOOLBOX_IMPORT_H
#define DSO_GUI_TOOLBOX_IMPORT_H

#include <Source/Core/BufferedIo.h>
#include <Source/Core/Serialize.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphBase.h>

class CImport {
 public:
  void LoadBinary(char *strName) {
    CBufferedReader reader;
    reader.Open(strName);

    for (int i = 0; i < (int)BIOS::ADC::GetCount(); i++) {
      BIOS::ADC::TSample &nValue = BIOS::ADC::GetAt(i);
      reader >> nValue;
    }
    reader.Close();
  }
  bool LoadWave(char *strName);
};

#endif
