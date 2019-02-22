#!/usr/bin/python

import annovar_modifications as a
import pandas as pd  


#Files needed in working folder
#1. This one
#2. annovar_modifications.py

#Calls currently available

#1. To convert an annovar annotated file into a restricted annovar annotated file (restriction determined by number (optional) and files of genes(required))
#a.find_genes_in_restriction(annovar_filepath, output_filepath, output_filepath2, restrictionlist = None, lowerLimit = 0, higherLimit = None)
    #annovar_filepath - source annovar file
    #restrictionlist - txt file of genes to focus on
    #output_filepath - location to export restriction file to
    #output_filepath2 - location to export negation of restriction file to
    #lowerLimit - lower limit of # of ids per variant
    #higherLimit - higher limit of # of ids with per variant

#2. To convert an annovar annotated file into a genome browser BED file with a restriction (restriction determined by number (optional) and files of genes(required))
#a.trackForGenesInRestriction(annovar_filepath, output_filepath, restrictionList = None, lowerLimit = 0, higherLimit = None)
    #annovar_filepath - source annovar file
    #restrictionlist - txt file of genes to focus on
    #output_filepath - location to export restriction genome browser file to
    #lowerLimit - lower limit of # of ids per variant
    #higherLimit - higher limit of # of ids with per variant

#3. To use an annovar annotated file to create a burden table with optional restrictions and different sorting methods (variant kind or gene supported)
#a.load_for_burden_tables(filepath,output_filepath,sort, restrictionList = None, lowerLimit = 0, higherLimit = None)
    #filepath - source annovar file
    #output_filepath - location to export burden table too
    #sort - kind of burden table (variant kind,gene, variant kind with gene, or chromosome)
    #restrictionList - txt file of genes to focus on (optional)
    #lowerLimit - lower limit of # of ids per variant (optional)
    #higherLimit - higher limit of # of ids per variant (optional)


#4. To use an annovar annotated file to a pheWAS input file with optional numerical and file restrictions
#a.create_PHEWAS_file(annovar_filepath, output_filepath, restrictionList = None,lowerLimit = 0, higherLimit = None)
    #annovar_filepath - source annovar file
    #output_filepath - location to export burden table too
    #restrictionList - txt file of genes to focus on
    #lowerLimit - lower limit of # of ids per variant
    #higherLimit - higher limit of # of ids per variant

#5. To convert an annovar annotated file into a CSV file with a restriction (restriction determined by number (optional) and files of genes(required)), 
#organized by ID and with CNVs and genes attached
#a.getCNVs(annovar_filepath, restrictionList, output_filepath, lowerLimit = 0, higherLimit = None)
    #annovar_filepath - source annovar file
    #restrictionlist - txt file of genes to focus on
    #output_filepath - location to export restriction genome browser file to
    #lowerLimit - lower limit of # of ids per variant
    #higherLimit - higher limit of # of ids with per variant

#SOURCES
#AFR_iCNV_raw_exonic_only.txt
#EURO_PMBB_CLAMMs_exonic_only.txt
#UPENN_UPENN_sample_EUR_permute_reduce_icnv_res_gb_chr_all_exonic_only.txt

#PREFIX
#AFR_iCNV_raw
#EURO_PMBB_CLAMMs
#UPENN_UPENN_sample_EUR_permute_reduce_icnv_res_gb_chr_all

#PATHS
#/Users/ashleynobi/Desktop/PythonWorkSpace/AFR_iCNV_raw_exonic_only.txt
#/Users/ashleynobi/Desktop/PythonWorkSpace/EURO_PMBB_CLAMMs_exonic_only.txt
#/Users/ashleynobi/Desktop/PythonWorkSpace/UPENN_UPENN_sample_EUR_permute_reduce_icnv_res_gb_chr_all_exonic_only.txt"


#a.trackForGenesInRestriction("/Users/ashleynobi/Desktop/PythonWorkSpace/exonic_cnv.txt","/Users/ashleynobi/Desktop/PythonWorkSpace/essentialgenes.txt", "/Users/ashleynobi/Desktop/PythonWorkSpace/exonic_cnv_essentialtrack_min2.txt","/Users/ashleynobi/Desktop/PythonWorkSpace/entryfile.txt",2,None)
#a.get_CNVs("/Users/CCXLoaner1/Desktop/ashleypython/exonic_cnv.txt","/Users/CCXLoaner1/Desktop/ashleypython/exonic_cnv_idandcnvlist.txt","/Users/CCXLoaner1/Desktop/ashleypython/essentialgenes.txt",2,None)
a.convertChrToEntry("/Users/ashleynobi/Desktop/PythonWorkspace/EURO_icnv_v2.txt","/Users/ashleynobi/Desktop/PythonWorkSpace/EURO_icnv_v2_sorted.txt")




