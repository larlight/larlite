/**
 * \file ROOTFileIO.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class ROOTFileIO
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_ROOTFILEIO_H
#define SELECTIONTOOL_ERTOOL_ROOTFILEIO_H

#include "IOBase.h"
#include <TChain.h>
#include <TFile.h>

namespace ertool {
  /**
     \class ROOTFileIO
     User defined class ROOTFileIO ... these comments are used to generate
     doxygen documentation!
  */
  class ROOTFileIO{
    
  public:
    
    /// Default constructor
    ROOTFileIO(const std::string& proc_name="no_name");
    
    /// Default destructor
    ~ROOTFileIO(){}

    /// Open input/output file(s)
    bool Open();

    /// Read a specified entry (event)
    bool ReadEntry(size_t entry);

    /// Write the current entry (event)
    bool WriteEntry();

    /// Close input/output file(s)
    bool Close();

  private:
    /// Input TChain handle
    TChain *_in_chain_edata, *_in_chain_pgraph, *_in_chain_edata_mc, *_in_chain_pgraph_mc;
    /// Output TTree handle
    TTree *_out_tree_edata, *_out_tree_pgraph, *_out_tree_edata_mc, *_out_tree_pgraph_mc;
    /// Output TFile handle
    TFile *_fout;
  };
}
  
#endif
/** @} */ // end of doxygen group 

