#!/usr/bin/python

import pandas as pd
import random

def createIDMapping(dataset):
	createIDMapping.id_dict = dict()
	createIDMapping.id_count_dict = dict()

	for i in range(dataset.shape[0]):
		row_dict = dict()
		count = 0
		
		str1 = str(dataset.iloc[i]["Id_homo"])
		str2 = str(dataset.iloc[i]["Id_het"])
		str3 = str(dataset.iloc[i]["Id_missing"])

		str1 = str1.replace('"'," ")
		str2 = str2.replace('"'," ")
		str3 = str3.replace('"'," ")

		homo_ids = str1.split(",")
		het_ids = str2.split(",")
		missing_ids = str3.split(",")
		
		
		for homo in homo_ids:
			row_dict[homo] = "homo"
			count += 1

		for het in het_ids:
			row_dict[het] = "het"
			count += 1

		for missing in missing_ids:
			row_dict[missing] = "missing"
			count += 1

		createIDMapping.id_dict[i] = row_dict
		createIDMapping.id_count_dict[i] = count

def kindOfColor(row):
	#return rgb coordinates from genotype
	if "homo"in row["type"]:
		return "0,0,0"
	elif "het" in row["type"]:
		return "0,0,0"
	elif "missing" in row["type"]:
		return "105,105,105"
	else:
		return "N/A"

def split_by_id(intervar_filepath, output_filepath):
	#load file
	df = pd.read_csv(intervar_filepath, sep="\t")
	
	#dataframe with id agg
	createIDMapping(df)

	del df["Id_homo"]
	del df["Id_het"]
	del df["Id_missing"]

	df["index"] = df.index

	df_next = pd.DataFrame(data=None, columns=df.columns)

	for i in range(df.shape[0]):
		count = createIDMapping.id_count_dict[i]
		#print(count)

		df_copy = df.iloc[i].copy()
		df_next = df_next.append([df_copy]*count,ignore_index=True)

	#df_Final.to_csv(output_filepath, sep='\t')

	df_next["id"] = ""
	df_next["type"] = ""
	curr_dict = createIDMapping.id_dict[0]
	
	for k in range(df_next.shape[0]):
		curr_index = df_next.iloc[k]["index"]
		curr_dict = createIDMapping.id_dict[curr_index]

		if bool(curr_dict):
			print(len(curr_dict))
			key = random.choice(curr_dict.keys())
			val = curr_dict[key]
			print(key)
			print(val)
			
			df_next.set_value(k,"id",key)
			df_next.set_value(k,"type",val)
			
			curr_dict.pop(key)  


	del df_next["index"]
	df_Final = df_next[df_next['id']!="nan"]
	
	df_Final.to_csv(output_filepath, sep='\t')

def create_track(intervar_filepath, output_filepath):
	#load file
	df = pd.read_csv(intervar_filepath, sep="\t")
	
	#dataframe with id agg
	createIDMapping(df)

	del df["Id_homo"]
	del df["Id_het"]
	del df["Id_missing"]

	df["index"] = df.index

	df_next = pd.DataFrame(data=None, columns=df.columns)

	for i in range(df.shape[0]):
		count = createIDMapping.id_count_dict[i]
		#print(count)

		df_copy = df.iloc[i].copy()
		df_next = df_next.append([df_copy]*count,ignore_index=True)

	#df_Final.to_csv(output_filepath, sep='\t')

	df_next["id"] = ""
	df_next["type"] = ""
	curr_dict = createIDMapping.id_dict[0]
	
	for k in range(df_next.shape[0]):
		curr_index = df_next.iloc[k]["index"]
		curr_dict = createIDMapping.id_dict[curr_index]

		if bool(curr_dict):
			print(len(curr_dict))
			key = random.choice(curr_dict.keys())
			val = curr_dict[key]
			print(key)
			print(val)
			
			df_next.set_value(k,"id",key)
			df_next.set_value(k,"type",val)
			
			curr_dict.pop(key)  


	del df_next["index"]
	df_next = df_next[df_next['id']!="nan"]
	df_next["empty"] = "."
	df_next["zero"] = "0"
	df_Final = df_next[['Chr','Start','End','id','zero','empty','zero','zero','type']].copy()
	df_Final["Chr"] = df_Final.apply(lambda x: "chr"+str(x["Chr"]), axis=1)
	df_Final["type"] = df_Final.apply(kindOfColor, axis=1)

	df_Final.to_csv(output_filepath, sep='\t',header=False,index=0)

def convertChrToEntry(filepath,output_filepath):
	dataset = pd.read_csv(filepath, sep="\t")

	dataset["Chr"] = dataset.apply(lambda x: str(x['Chr']) + ":"  + str(x['Start']) + "-" + str(x['End']),axis=1)
	dataset = dataset.rename(columns = {"Chr":"entry"})

	fdataset = dataset[['entry','Ref.Gene','Id_homo','Id_het','Id_missing']].copy()
	fdataset.sort_values(by=["entry"])
	fdataset.to_csv(output_filepath, sep='\t',index=False)