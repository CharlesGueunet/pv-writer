#include "vtkGenericWriter.h"

#include "vtkDataSet.h"
#include "vtkDirectory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtksys/SystemTools.hxx"

vtkStandardNewMacro(vtkGenericWriter);

// Public
// {{{

//----------------------------------------------------------------------------
void vtkGenericWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

// }}}
// Protected
// {{{

//----------------------------------------------------------------------------
vtkGenericWriter::vtkGenericWriter()
{
  this->FileName = "";
}

//----------------------------------------------------------------------------
vtkGenericWriter::~vtkGenericWriter()
{
}

int vtkGenericWriter::FillOutputPortInformation(int port, vtkInformation* info)
{
  switch (port)
    {
    case 0:
      info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkDataSet");
      break;
    default:
      info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkDataObject");
      break;
    }

  return 1;
}

void vtkGenericWriter::WriteCSVFile(vtkDataSet* inputData) const
{
  vtkPointData* inputPointData = inputData->GetPointData();
  const int numberOfArrays = inputPointData->GetNumberOfArrays();

  vtkSmartPointer<vtkTable> table=vtkSmartPointer<vtkTable>::New();
  for(int i = 0; i < numberOfArrays; ++i) {
    vtkDataArray* arr = inputPointData->GetArray(i);

    if(arr)
      table->AddColumn(arr);
  }

  vtkSmartPointer<vtkDelimitedTextWriter> writer = vtkSmartPointer<vtkDelimitedTextWriter>::New();
  writer->SetFileName(FileName.c_str());
  writer->SetInputData(table);

  writer->Write();
}

int vtkGenericWriter::RequestData(vtkInformation*        request,
                                  vtkInformationVector** inputVector,
                                  vtkInformationVector*  outputVector)
{

  vtkDataSet* inputData = vtkDataSet::GetData(inputVector[0]);

  vtkInformation* outputInfo = outputVector->GetInformationObject(0);
  vtkDataSet*     outputData =
      vtkDataSet::SafeDownCast(outputInfo->Get(vtkDataObject::DATA_OBJECT()));
  // this plugin does not modify the input
  outputData->ShallowCopy(inputData);

  if (FileName == "")
    {
      // Default name
      static unsigned occurenceNoName = 0;
      FileName = "SavedData_" + std::to_string(occurenceNoName++);
    }
  else
    {
      vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
      if (dir->FileIsDirectory(FileName.c_str()))
        {
          vtkErrorMacro("You should not pass a directory as output filename");
          return 0;
        }
    }

  // choose writer and save on disk
  std::string fileExtension =
      vtksys::SystemTools::GetFilenameLastExtension(FileName);

// could use vtkDebugMacro
#ifdef WRITER_DEBUG
  if (fileExtension == "")
    {
      std::cout << "no extension, will guess" << std::endl;
    }
  else
    {
      std::cout << "found extension : " << fileExtension << std::endl;
    }
#endif

  if (fileExtension == "")
    {
      if (inputData->GetDataObjectType() == VTK_UNSTRUCTURED_GRID)
        {
          fileExtension = ".vtu";
        }
      else if (inputData->GetDataObjectType() == VTK_IMAGE_DATA)
        {
          fileExtension = ".vti";
        }
      else if (inputData->GetDataObjectType() == VTK_POLY_DATA)
        {
          fileExtension = ".vtp";
        }
      FileName += fileExtension;
    }

#ifdef WRITER_DEBUG
  std::cout << "Data will be saved in : " << FileName << std::endl;
#endif

  if (fileExtension == ".vtu")
    {
      WriteXMLFile<vtkXMLUnstructuredGridWriter>(inputData);
    }
  else if (fileExtension == ".vti")
    {
      WriteXMLFile<vtkXMLImageDataWriter>(inputData);
    }
  else if (fileExtension == ".vtp")
    {
      WriteXMLFile<vtkXMLPolyDataWriter>(inputData);
    }
  else if (fileExtension == ".csv")
    {
      WriteCSVFile(inputData);
    }
  else
    {
      vtkErrorMacro("Unsupported DataType, nothing have been written!");
      return 0;
    }

  return 1;
}

// }}}
