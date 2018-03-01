// .NAME vtkGenericWriter - save a dataset on disc
// .SECTION Description
// vtkGenericWriter is a filter to save a dataset on the disk
// while using it. A typical example is saving a plane with
// random scalar-field when tracking a bug on a plugin.
// It will allow you to keep the problematic data-set.

#ifndef __vtkGenericWriter_h
#define __vtkGenericWriter_h

#include <string>

#include "vtkDataSetAlgorithm.h"
#include "vtkSmartPointer.h"

// writers
#include "vtkXMLImageDataWriter.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkDelimitedTextWriter.h"
#include "vtkTable.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

class VTK_EXPORT vtkGenericWriter : public vtkDataSetAlgorithm
{
public:
  static vtkGenericWriter* New();
  vtkTypeMacro(vtkGenericWriter, vtkDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetMacro(FileName, std::string);
  vtkGetMacro(FileName, std::string);

protected:
  vtkGenericWriter();
  ~vtkGenericWriter();

  // VTK Interface

  virtual int FillOutputPortInformation(int port, vtkInformation* info);

  int RequestData(vtkInformation*        request,
                  vtkInformationVector** inputVector,
                  vtkInformationVector*  outputVector);

  // methods
  template <class TWriter> void WriteXMLFile(vtkDataSet* inputData) const;
  void WriteCSVFile(vtkDataSet* inputData) const;

  // fields

  std::string FileName;

private:
  vtkGenericWriter(const vtkGenericWriter&); // Not implemented.
  void operator=(const vtkGenericWriter&);   // Not implemented.
};

// Template functions
template <class TWriter>
void vtkGenericWriter::WriteXMLFile(vtkDataSet* inputData) const
{
  vtkSmartPointer<TWriter> writer = vtkSmartPointer<TWriter>::New();
  writer->SetFileName(FileName.c_str());
  writer->SetInputData(inputData);

  writer->Write();
}

#endif
