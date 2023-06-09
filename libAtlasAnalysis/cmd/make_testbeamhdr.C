void make_testbeamhdr(const char* InputFile,
		     const char* ClassName = "AtlTestBeamProxy") {
    //
    // Generate .h and .cxx file for given Combined Test Beam A++ ntuple with
    // the help of the TTree::MakeProxy() facility
    //

    // Paths and filenames
    TString IncludePath("../inc");
    TString SourcePath("../src");
    TString HeaderFile = TString(ClassName) + ".h";
    TString HeaderFileLong = TString(IncludePath) + "/" + HeaderFile;
    TString SourceFile = TString(ClassName) + ".cxx";
    TString SourceFileLong = TString(SourcePath) + "/" + SourceFile;
    TString HeaderTmp = TString(HeaderFile) + "_tmp";
    TString MacroFile = TString(ClassName) + "_macro.C";
    TString MacroFileLong = TString(IncludePath) +  "/" + MacroFile;
    
    // Check wether the header file exists already
    // and ask for permission to overwrite
    if ( gSystem->AccessPathName(HeaderFileLong.Data()) == kFALSE ) {
	TString input = "";
	cout << endl
	     << "File " << HeaderFileLong.Data()
	     << " is already exisiting." << endl
	     << "Really overwrite? (yY)> ";
	input.ReadToDelim(cin);
	if ( !input.Contains("y", TString::kIgnoreCase) &&
	     input.Length() > 0 ) {
	    cout << "Exiting." << endl;
	    gSystem->Abort(0);
	}
    }

    // Write dummy macro
    ofstream macro;
    macro.open(MacroFile.Data());
    macro << "Double_t " << ClassName << "_macro() {" << endl
	  << "return 0.;" << endl
	  << "}" << endl;
    macro.close();

    // Open input file and fetch trees
    TFile *f = new TFile(InputFile, "read");
    TTree *tree = (TTree*)gDirectory->Get("tree");

    // Make proxy
    tree->MakeProxy(ClassName, MacroFile.Data());

    // Close input
    delete f;
    
    // Get user's full name
    TString user;
    FILE *fp = gSystem->OpenPipe("finger -s `whoami` | tail -1 | awk -F\"    \" '{print $2}'", "r");
    user.Gets(fp);
    gSystem->ClosePipe(fp);

    // Include file header
    TString file_hdr = TString("//
// $Id$
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!                                               !!
// !!   D O   N O T   E D I T   T H I S   F I L E   !!
// !!                                               !!
// !! This file has been generated by the script    !!
// !! cmd/make_testbeamhdr with the help of the     !!
// !! TTree::MakeProxy() functionality              !!
// !!                                               !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//       created by user ") + user.Data() + "
//

";

    //
    // Disect header file into .h and .cxx file
    //

    // Open output and prepend header
    ofstream out;
    out.open(HeaderFileLong.Data());
    out << file_hdr.Data();

    // Regexpt for removing inline statements
    TRegexp re1("inline ");

    // Dtor search strings
    TString dtor_str = TString("~") + ClassName;
    TString split_str = TString("inline ") + ClassName + "::~" + ClassName;
    
    // Open input header file and read line by line
    TString line;
    ifstream hdr_in;
    hdr_in.open(HeaderFile.Data());
    Bool_t hdr = kTRUE;
    while ( !hdr_in.eof() ) {
	line.ReadLine(hdr_in);

	// When the dtor is reached redirect output to the .cxx file
	if ( line.Contains(split_str.Data()) ) {
	    hdr = kFALSE;
	    out.close();
	    out.open(SourceFileLong.Data());

	    // Prepend header
	    out << file_hdr.Data() << endl;

	    // Add include statement
	    out << "#ifndef " << ClassName << "_h" << endl
		<< "#include <" << ClassName << ".h>" << endl
		<< "#endif" << endl << endl;
	}

	// Remove all inline statements
	line(re1) = "";

 	// Declare dtor as well as all member functions found also in
	// TSelector as virtual
	TString prep;
	if ( hdr ) {
	    if ( line.Contains(dtor_str) ) {
		prep = TString("virtual ");
	    } else {
		TIter next_method(TSelector::Class()->GetListOfMethods());
		TMethod *method = 0;
		while ( method = (TMethod*)next_method() ) {
		    TString search_str = TString(" ") + method->GetName()
			+ "(";
		    if ( line.Contains(search_str) ) {
			prep = TString("virtual ");
			break;
		    } else {
			prep = TString("");
		    }
		}
	    }
	}

	// Write output
	out << prep.Data() << line.Data() << endl;
    }

    // Close i/o streams
    out.close();
    hdr_in.close();

    // Copy the macro dummy to the include directory
    gSystem->Exec(Form("mv %s %s", MacroFile.Data(), MacroFileLong.Data()));
		     
    // Clean-up
    gSystem->Exec(Form("rm -f %s", HeaderFile.Data()));

    // Finish
    cout << endl
	 << "Finished !" << endl
	 << "Use 'make clean lib' now to re-compile the library." << endl;
}
