

import intervar_modifications as i
import pandas as pd  


#Files needed in working folder
#1. This one
#2. intervar_modifications.py

#Calls currently available

#1. To convert an intervar file into an intervar file with unique lines for each id
#split_by_id(intervar_filepath, output_filepath):
    #annovar_filepath - source annovar file
    #output_filepath - location to export edited file to


#SOURCES
#jing_intervar_input


#PREFIX
#AFR_iCNV_raw
#EURO_PMBB_CLAMMs
#UPENN_UPENN_sample_EUR_permute_reduce_icnv_res_gb_chr_all

#PATHS
#/Users/ashleynobi/Desktop/PythonWorkSpace/jing_intevar_input.txt


#a.trackForGenesInRestriction("/Users/ashleynobi/Desktop/PythonWorkSpace/exonic_cnv.txt","/Users/ashleynobi/Desktop/PythonWorkSpace/essentialgenes.txt", "/Users/ashleynobi/Desktop/PythonWorkSpace/exonic_cnv_essentialtrack_min2.txt","/Users/ashleynobi/Desktop/PythonWorkSpace/entryfile.txt",2,None)
#a.get_CNVs("/Users/CCXLoaner1/Desktop/ashleypython/exonic_cnv.txt","/Users/CCXLoaner1/Desktop/ashleypython/exonic_cnv_idandcnvlist.txt","/Users/CCXLoaner1/Desktop/ashleypython/essentialgenes.txt",2,None)
i.create_track("/Users/ashleynobi/Desktop/PythonWorkSpace/jing_intervar_input.txt", "/Users/ashleynobi/Desktop/PythonWorkSpace/intervartest2.txt")
i.convertChrToEntry("/Users/ashleynobi/Desktop/PythonWorkSpace/jing_intervar_input.txt", "/Users/ashleynobi/Desktop/PythonWorkSpace/intervartest2_entry.txt")