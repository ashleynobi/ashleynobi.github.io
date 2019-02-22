#!/usr/bin/python

import pandas as pd

def hasTheGene(row):
	str2 = row["genes"].replace('"'," ")
	genelist = str2.split(",")

	for gene in genelist:
		if gene in storage.genes:
			for word in storage.words:
				if gene == word:
					#print(gene + " matches " + word)
					return True

	return False

def storage(restrictionList):
	restrict = open(restrictionList)
	storage.genes = restrict.read()
	storage.words = storage.genes.split()

def kindOfVariant(row):
	print(row["color"])
	#return values from rgb coordinates
	if "128,0,0" in row["color"]:
		return "Homozygous Deletion"
	elif "130,0,0" in row["color"]:
		return "Homozygous Deletion"
	elif "0,128,0" in row["color"]:
		return "Homozygous Duplication"
	elif "0,130,0" in row["color"]:
		return "Homozygous Duplication"
	elif "255,0,0" in row["color"]:
		return "Heterozygous Deletion"
	elif "0,255,0" in row["color"]:
		return "Heterozygous Duplication"
	else:
		return "N/A"

def kindOfColor(row):
	#return rgb coordinates from genotype
	if "Homozygous Deletion"in row["Kind Of Variant"]:
		return "128,0,0" 
	elif "Homozygous Duplication" in row["Kind Of Variant"]:
		return "0,128,0"
	elif "Heterozygous Deletion" in row["Kind Of Variant"]:
		return "255,0,0"
	elif "Heterozygous Duplication" in row["Kind Of Variant"]:
		return "0,255,0" 
	else:
		return "N/A"

def kindOfVariantDirect(color):
	#return values from rgb coordinates (as a string, not series entry)
	if "128,0,0" in color:
		return "Homozygous Deletion"
	elif "130,0,0" in color:
		return "Homozygous Deletion"
	elif "0,128,0" in color:
		return "Homozygous Duplication"
	elif "0,130,0" in color:
		return "Homozygous Duplication"
	elif "255,0,0" in color:
		return "Heterozygous Deletion"
	elif "0,255,0" in color:
		return "Heterozygous Duplication"
	else:
		return "N/A"

def createKey(chromo, start, finish, kind):
	#concatenate values into string
	key = str(chromo)
	key += '_'
	key += str(start)
	key += '-'
	key += str(finish)

    #variant specificity
	if kind == "Homozygous Deletion":
		key += '_'
		key += "HoDel"
	elif kind == "Homozygous Duplication":
		key += '_'
		key += "HoDup"
	elif kind == "Heterozygous Deletion":
		key += '_'
		key += "HeDel"
	elif kind == "Heterozygous Duplication":
		key += '_'
		key += "HeDup"
	else:
		return key

	return key

def modifyDatasetWithLimits(dataset, lowerLimit, higherLimit):
	#find id count per gene
	id_count_gb = dataset.groupby(["chr", "start", "finish","Kind Of Variant"]) ["id"].count()
	id_count_dataset = id_count_gb.reset_index(name='id_count')

	#restricted dataframe and set for associated keys

	commongenes = id_count_dataset[id_count_dataset['id_count']>=lowerLimit]
	if not higherLimit is None:
		commongenes = commongenes[commongenes['id_count']<=higherLimit]

	num_keys = set()
	
	#adding keys to set for comparison
	for i in range(commongenes.shape[0]):
		k = createKey(commongenes.iloc[i]["chr"],commongenes.iloc[i]["start"], commongenes.iloc[i]["finish"],commongenes.iloc[i]["Kind Of Variant"])
		num_keys.add(k)

	#finding rows that don't fit into restriction number
	rows_to_delete = set()
	for j in range(dataset.shape[0]):
		k = createKey(dataset.iloc[j]["chr"],dataset.iloc[j]["start"], dataset.iloc[j]["finish"],dataset.iloc[j]["Kind Of Variant"])
			
		if k not in num_keys:
			#print("remove")
			rows_to_delete.add(j)

	rows_to_delete_final = list(rows_to_delete)
	dataset = dataset.drop(dataset.index[rows_to_delete_final])

	return dataset

def createEntryFile(dataset, lowerLimit, higherLimit,output_filepath):
	#find id count per gene
	id_count_gb = dataset.groupby(["chr", "start", "finish","Kind Of Variant"]) ["id"].count()
	id_count_dataset = id_count_gb.reset_index(name='id_count')

	#restricted dataframe and set for associated keys

	commongenes = id_count_dataset[id_count_dataset['id_count']>=lowerLimit]
	
	if not higherLimit is None:
		commongenes = commongenes[commongenes['id_count']<=higherLimit]

	entrydb = commongenes.copy()
	entrydb['entry'] = entrydb.apply(lambda x: "chr" + str(x['chr']) + ":"  + str(x['start']) + "-" + str(x['finish']), axis=1)
	

	entries = entrydb['entry']
	entrydb.drop(labels=['entry'], axis=1,inplace = True)
	entrydb.insert(0, 'entry', entries)

	del entrydb["chr"]
	del entrydb["start"]
	del entrydb["finish"]
	del entrydb["Kind Of Variant"]

	entrydb = entrydb.sort_values(by=['id_count'],ascending=False)

	entrydb.to_csv(output_filepath, sep ="\t", index = False, header = False)

def createEntry(chromo,start,finish):
	return "chr" + str(chromo) + ":"  + str(start) + "-" + str(finish)

def removeExtra(row):
	if "128,0,0" in row["color"]:
		return "128,0,0"
	elif "0,128,0" in row["color"]:
		return "0,128,0"
	elif "255,0,0"in row["color"]:
		return "255,0,0"
	elif "0,255,0" in row["color"]:
		return "0,255,0"
	else:
		return "0,0,0"

def conversionDict(conv_filepath,direction):
	conv_dataset = pd.read_csv(conv_filepath,sep ="\t", low_memory = False)
	conversionDict.iDconversionDict = dict()
	conv_columns = list(conv_dataset)
	#print(conv_columns)

	#create reference dictionary for long ids
	if direction == "standard":
		for k in range(conv_dataset.shape[0]):
			before = conv_dataset.iloc[k][conv_columns[0]]
			after = conv_dataset.iloc[k][conv_columns[1]]
			conversionDict.iDconversionDict[before] = after
	if direction == "reverse":
		for k in range(conv_dataset.shape[0]):
			before = conv_dataset.iloc[k][conv_columns[1]]
			after = conv_dataset.iloc[k][conv_columns[0]]
			conversionDict.iDconversionDict[before] = after
	else:
		print("direction not supported")

def shortToLong(row):
	#modifications so that ids match short_id in file
	cpy = row['id']

	if not "UPENN_UPENN" in cpy:
		cpy = "UPENN_"
		cpy += row['id']

	#change id to long id
	return conversionDict.iDconversionDict[cpy]

def idData(dataset):
	ids = set()
	for i in range(dataset.shape[0]):
		id = dataset.iloc[i]["id"]
		ids.add(id)
	
	ids_list = list(ids)
	ids_dict =  {'id': ids_list}
	idData.ids_df = pd.DataFrame(ids_dict)

	idData.id_index = dict()
	for j in range(idData.ids_df.shape[0]):
		idData.id_index[idData.ids_df.iloc[j]["id"]] = j
	
def updateRestrictedCNVs(num,data, dictionary):
	genes = data.iloc[num]["genes"]
	start2 = data.iloc[num]["start2"]
	finish2 = data.iloc[num]["finish2"]

	chromo = data.iloc[num]["chr"]
	start = data.iloc[num]["start"]
	finish = data.iloc[num]["finish"]
	kind = data.iloc[num]["Kind Of Variant"]

	#create unique identifier for each variant
	data_key = createKey(chromo,start,finish,kind)

	#create dictionary pair 
	dictionary[data_key] = [genes,chromo,start,finish,start2,finish2,kind]

	return dictionary

def importantGenes(row):
	str2 = row["genes"].replace('"'," ")
	genelist = str2.split(",")
	newStr = str()

	for gene in genelist:
		if gene in storage.genes:
			for word in storage.words:
				if gene == word:
					newStr += gene
					newStr += ", "

	return newStr

def wordCount(row):
	str2 = row["entries"].replace('"'," ")
	entrylist = str2.split(",")
	count = 0

	for entry in entrylist:
		count += 1

	return count




def find_genes_in_restriction(annovar_filepath,  restrictionlist, output_filepath, output_filepath2, lowerLimit = 0, higherLimit = None):
	#load file
	dataset= pd.read_csv(annovar_filepath, sep="\t", header=None, names=["type of region", "genes", "chr", "start", "finish", "-1","-2","id","-3","-4","start2","finish2","color"])
		
	#update global variables
	storage(restrictionlist)

	#find id count per gene
	g1 = dataset.groupby(["genes", "chr", "start", "finish", "start2","finish2","color"]) ["id"].count()

	#dataframe with id count
	with_id_count = g1.reset_index(name='id_count')

	#add numerical restriction
	commongenes = with_id_count[with_id_count['id_count']>=lowerLimit]

	if not higherLimit == None:
		commongenes_copy = commongenes[commongenes['id_count']<=higherLimit]
		commongenes = commongenes_copy

	#change rgb code to kind of variant
	commongenes["color"] = commongenes.apply(kindOfVariant, axis=1)
	commongenes = commongenes.rename(columns = {"color":"Kind Of Variant"})

	#find boolean restriction
	commongenes['on gene list?'] = commongenes.apply(hasTheGene, axis=1)

	#dataframe with appropriate restrictions
	experimental = commongenes[commongenes['on gene list?']]
	experimental_neg = commongenes[commongenes['on gene list?'] == False]

	del experimental["on gene list?"]
	del experimental_neg["on gene list?"]

	#print(experimental.head(5))
	
	experimental.to_csv(output_filepath, sep='\t')
	experimental_neg.to_csv(output_filepath2, sep='\t')

def trackForGenesInRestriction(annovar_filepath, output_filepath, restrictionList, output_filepath2, lowerLimit = 0, higherLimit = None):
	dataset = pd.read_csv(annovar_filepath, sep="\t", header=None, names=["type of region", "genes", "chr", "start", "finish", "-1","-2","id","-3","-4","start2","finish2","color"])
	#change rgb code to kind of variant
	dataset["color"] = dataset.apply(kindOfVariant, axis=1)
	dataset= dataset.rename(columns = {"color":"Kind Of Variant"})


	if not restrictionList == None:
		storage(restrictionList)
		#find boolean restriction
		dataset['on gene list?'] = dataset.apply(hasTheGene, axis=1)

		#dataframe with appropriate restrictions
		g_dataset = dataset[dataset['on gene list?']]
		dataset = g_dataset

	createEntryFile(dataset,lowerLimit,higherLimit,output_filepath2)

	if not lowerLimit == 0 or not higherLimit == None:
	    dataset = modifyDatasetWithLimits(dataset, lowerLimit, higherLimit)
	    

	dataset["Kind Of Variant"] = dataset.apply(kindOfColor, axis=1)
	dataset= dataset.rename(columns = {"Kind Of Variant":"color"})
	dataset["color"] = dataset.apply(removeExtra, axis=1)
	dataset["chr"] = dataset.apply(lambda x: "chr"+str(x["chr"]), axis=1)
	dataset["-4"] = dataset.apply(lambda x: ".", axis=1) 


	del dataset["-1"]
	del dataset["-2"]
	del dataset["type of region"]
	del dataset["genes"]
	del dataset["on gene list?"]

	dataset.to_csv(output_filepath, sep='\t',header=False,index=0)


def buildBurdenByKindTable(dataset):

	#create default column
	emptylist = list()

	for _ in range(idData.ids_df.shape[0]):
		emptylist.append(0)
	
	#create all columns as a default and create dictionary
	list1 = list(emptylist)
	list2 = list(emptylist)
	list3 = list(emptylist)
	list4 = list(emptylist)
	list5 = list(emptylist)
	list6 = list(emptylist)
	list7 = list(emptylist)
	list8 = list(emptylist)
	list9 = list(emptylist)
	list10 = list(emptylist)

	burden_dict = {'HomoDelCount':list1, 'HomoDelLength': list2,'HomoDupCount': list3,
	'HomoDupLength': list4,'HetDelCount': list5,'HetDelLength': list6,
	'HetDupCount': list7,'HetDupLength': list8, 'N/A Count':list9, 'N/A Length': list10}
	
	#modify dictionary in place based on variant kind
	for j in range(dataset.shape[0]):
		index = idData.id_index[dataset.iloc[j]["id"]]
		kind = dataset.iloc[j]["Kind Of Variant"]

		if kind == "Homozygous Deletion":
			klist1 = burden_dict['HomoDelCount']
			klist2 = burden_dict['HomoDelLength']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			burden_dict['HomoDelCount'] = klist1
			burden_dict['HomoDelLength'] = klist2
			#print("1")
		elif kind == "Homozygous Duplication":
			klist1 = burden_dict['HomoDupCount']
			klist2 = burden_dict['HomoDupLength']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			burden_dict['HomoDupCount'] = klist1
			burden_dict['HomoDupLength'] = klist2
			#print("2")
		elif kind ==  "Heterozygous Deletion":
			klist1 = burden_dict['HetDelCount']
			klist2 = burden_dict['HetDelLength']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			burden_dict['HetDelCount'] = klist1
			burden_dict['HetDelLength'] = klist2
			#print("3")
		elif kind == "Heterozygous Duplication":
			klist1 = burden_dict['HetDupCount']
			klist2 = burden_dict['HetDupLength']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			burden_dict['HetDupCount'] = klist1
			burden_dict['HetDupLength'] = klist2
			#print("4")
		elif kind == "N/A":
			klist1 = burden_dict['N/A Count']
			klist2 = burden_dict['N/A Length']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			burden_dict['N/A Count'] = klist1
			burden_dict['N/A Length'] = klist2
			#print("5")

	burden_data = pd.DataFrame(burden_dict)
	return burden_data
		
def buildBurdenbyGeneTable(dataset):

	genes = set()
	emptylist = list()
	burden_dict = dict()
	
	#create template for new columsn
	for _ in range(idData.ids_df.shape[0]):
		emptylist.append(0)
	

	for j in range (dataset.shape[0]):
		index = idData.id_index[dataset.iloc[j]["id"]]
		#getting every gene covered for CNVs
		genes2 = dataset.iloc[j]["genes"]
		genes2 = genes2.replace('"'," ")
		geneslist = genes2.split(",")

		
		for g in geneslist:
			#column names for comparision or creation
			column_name1 = g
			column_name1 += "_variantcount"
			column_name2 = g
			column_name2 += "_variantlength"

			if not g in genes:
				genes.add(g)
				glist1 = list(emptylist)
				glist2 = list(emptylist)

				glist1[index] += 1
				glist2[index] += dataset.iloc[j]["length"]
				
				burden_dict[column_name1] = glist1
				burden_dict[column_name2] = glist2
			else:
				glist1 = burden_dict[column_name1]
				glist2 = burden_dict[column_name2]

				glist1[index] += 1
				glist2[index] += dataset.iloc[j]["length"]

				burden_dict[column_name1] = glist1
				burden_dict[column_name2] = glist2

	burden_data = pd.DataFrame(burden_dict)
	return burden_data

def buildBurdenbyChromosomeTable(dataset):
	chromosomes = list(['chr1','chr2','chr3','chr4','chr5','chr6','chr7','chr8','chr9','chr10','chr11','chr12','chr13','chr14','chr15','chr16','chr17','chr18','chr19','chr20','chr21','chr22','chr23'])
	emptylist = list()
	burden_dict = dict()
	
	#create template for new columns
	for _ in range(idData.ids_df.shape[0]):
		emptylist.append(0)

	for chromo in chromosomes:
		column_name1 = chromo
		column_name1 += "_variantcount"
		column_name2 = chromo
		column_name2 += "_variantlength"

		glist1 = list(emptylist)
		glist2 = list(emptylist)

		burden_dict[column_name1] = glist1
		burden_dict[column_name2] = glist2
	

	for j in range (dataset.shape[0]):
		index = idData.id_index[dataset.iloc[j]["id"]]
		#getting every gene covered for CNVs
		chromosome = dataset.iloc[j]["chr"]

		column_name1 = chromosome
		column_name1 += "_variantcount"
		column_name2 = chromosome
		column_name2 += "_variantlength"

		glist1 = burden_dict[column_name1]
		glist2 = burden_dict[column_name2]

		glist1[index] += 1
		glist2[index] += dataset.iloc[j]["length"]

		burden_dict[column_name1] = glist1
		burden_dict[column_name2] = glist2

	burden_data = pd.DataFrame(burden_dict)
	return burden_data

def buildBurdenByKindWithGeneTable(dataset):

	#create default column
	emptylist = list()
	emptylist_txt = list()

	for _ in range(idData.ids_df.shape[0]):
		emptylist.append(0)
		emptylist_txt.append("")
	
	#create all columns as a default and create dictionary
	list1 = list(emptylist)
	list2 = list(emptylist)
	list3 = list(emptylist_txt)
	list4 = list(emptylist)
	list5 = list(emptylist)
	list6 = list(emptylist_txt)
	list7 = list(emptylist)
	list8 = list(emptylist)
	list9 = list(emptylist_txt)
	list10 = list(emptylist)
	list11 = list(emptylist)
	list12 = list(emptylist_txt)
	list13 = list(emptylist)
	list14 = list(emptylist)
	list15 = list(emptylist_txt)
	list16 = list(emptylist_txt)

	burden_dict = {'HomoDelCount':list1, 'HomoDelLength': list2,'HomoDelGenes': list3,
	'HomoDupCount': list4, 'HomoDupLength': list5,'HomoDupGenes': list6,
	'HetDelCount': list7,'HetDelLength': list8, 'HetDelGenes': list9,
	'HetDupCount': list10,'HetDupLength': list11, 'HetDupGenes': list12, 
	'N/A Count':list13, 'N/A Length': list14, 'N/A Genes': list15, 'All Genes':list16}

	HomoDelGenes = set()
	HomoDupGenes = set()
	HetDelGenes = set()
	HetDupGenes = set()
	NA_Genes = set()
	
	#modify dictionary in place based on variant kind
	for j in range(dataset.shape[0]):
		index = idData.id_index[dataset.iloc[j]["id"]]
		kind = dataset.iloc[j]["Kind Of Variant"]

		genes = dataset.iloc[j]["genes"]
		genes = genes.replace('"'," ")
		geneslist = genes.split(",")

		klist = burden_dict["All Genes"]
		for g in geneslist:
			if g not in klist[index]:
				klist[index] += g
				klist[index] += ", "

		if kind == "Homozygous Deletion":
			klist1 = burden_dict['HomoDelCount']
			klist2 = burden_dict['HomoDelLength']
			klist3 = burden_dict['HomoDelGenes']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			for g in geneslist:
				HomoDelGenes.add(g)
				if g not in klist3[index]:
					klist3[index] += g
					klist3[index] += ", "

			burden_dict['HomoDelCount'] = klist1
			burden_dict['HomoDelLength'] = klist2
			burden_dict['HomoDelGenes'] = klist3

		elif kind == "Homozygous Duplication":
			klist1 = burden_dict['HomoDupCount']
			klist2 = burden_dict['HomoDupLength']
			klist3 = burden_dict['HomoDupGenes']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			for g in geneslist:
				HomoDupGenes.add(g)
				if g not in klist3[index]:
					klist3[index] += g
					klist3[index] += ", "

			burden_dict['HomoDupCount'] = klist1
			burden_dict['HomoDupLength'] = klist2
			burden_dict['HomoDupGenes'] = klist3

		elif kind ==  "Heterozygous Deletion":
			klist1 = burden_dict['HetDelCount']
			klist2 = burden_dict['HetDelLength']
			klist3 = burden_dict['HetDelGenes']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			for g in geneslist:
				if g not in klist3[index]:
					HetDelGenes.add(g)
					klist3[index] += g
					klist3[index] += ", "

			burden_dict['HetDelCount'] = klist1
			burden_dict['HetDelLength'] = klist2
			burden_dict['HetDelGenes'] = klist3

		elif kind == "Heterozygous Duplication":
			klist1 = burden_dict['HetDupCount']
			klist2 = burden_dict['HetDupLength']
			klist3 = burden_dict['HetDupGenes']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]

			for g in geneslist:
				HetDupGenes.add(g)
				if g not in klist3[index]:
					klist3[index] += g
					klist3[index] += ", "

			burden_dict['HetDupCount'] = klist1
			burden_dict['HetDupLength'] = klist2
			burden_dict['HetDupGenes'] = klist3

		elif kind == "N/A":
			klist1 = burden_dict['N/A Count']
			klist2 = burden_dict['N/A Length']
			klist3 = burden_dict['N/A Genes']

			klist1[index] += 1
			klist2[index] += dataset.iloc[j]["length"]
			for g in geneslist:
				NA_Genes.add(g)
				if g not in klist3[index]:
					klist3[index] += g
					klist3[index] += ", "

			burden_dict['N/A Count'] = klist1
			burden_dict['N/A Length'] = klist2
			burden_dict['N/A Genes'] = klist3

	genesummary = "Homozygous Deletions"
	for g1 in HomoDelGenes:
		genesummary += g1
		genesummary += ", "
	genesummary += "\n\n\n"
	genesummary += "Homozygous Duplications"
	for g2 in HomoDupGenes:
		genesummary += g2
		genesummary += ", "
	genesummary += "\n\n\n"
	genesummary += "Heterozygous Deletions"
	for g3 in HetDelGenes:
		genesummary += g3
		genesummary += ", "
	genesummary += "\n\n\n"
	genesummary += "Heterozygous Duplications"
	for g4 in HetDupGenes:
		genesummary += g4
		genesummary += ", "
	genesummary += "\n\n\n"
	genesummary += "N/A"
	for g5 in NA_Genes:
		genesummary += g5
		genesummary += ", "

	f= open("/Users/ashleynobi/Desktop/genesummary.txt","w+")
	f.write(genesummary)

	burden_data = pd.DataFrame(burden_dict)
	return burden_data

def load_for_burden_tables(filepath,output_filepath,sort, restrictionList = None, lowerLimit = 0, higherLimit = None):
	dataset= pd.read_csv(filepath, sep="\t", header=None, names=["type of region", "genes", "chr", "start", "finish", "-1","-2","id","-3","-4","start2","finish2","color"])

	dataset["color"] = dataset.apply(kindOfVariant, axis=1)
	dataset = dataset.rename(columns = {"color":"Kind Of Variant"})
	dataset["length"] = dataset.apply(lambda row: (row["finish"] - row["start"]), axis=1)

	if not lowerLimit == 0 or not higherLimit == None:
		dataset = modifyDatasetWithLimits(dataset, lowerLimit, higherLimit)

	#dataset.to_csv("/Users/ashleynobi/Desktop/AFR_iCNV_raw_numerical_restriction.txt",sep='\t')
					
	if not restrictionList == None:
		storage(restrictionList)
		#find boolean restriction
		dataset['on gene list?'] = dataset.apply(hasTheGene, axis=1)

		#dataframe with appropriate restrictions
		final_dataset = dataset[dataset['on gene list?']]
		dataset = final_dataset

	#dataset.to_csv("/Users/ashleynobi/Desktop/AFR_iCNV_raw_numericalandfile_restriction.txt",sep='\t')
	
	idData(dataset)

	if sort == "variant kind":
		burden_table = buildBurdenByKindTable(dataset)
		#insert common id column
		burden_table.insert(loc=0, column='id', value=idData.ids_df)
		burden_table.to_csv(output_filepath, sep='\t')
	elif sort == "gene":
		burden_table = buildBurdenbyGeneTable(dataset)
		#insert common id column
		burden_table.insert(loc=0, column='id', value=idData.ids_df)
		burden_table.to_csv(output_filepath, sep='\t')
	elif sort == "chromosome":
		burden_table =  buildBurdenbyChromosomeTable(dataset)
		#insert common id column
		burden_table.insert(loc=0, column='id', value=idData.ids_df)
		burden_table.to_csv(output_filepath, sep='\t')
	elif sort == "variant kind with gene":
		burden_table = buildBurdenByKindWithGeneTable(dataset)
		#insert common id column
		burden_table.insert(loc=0, column='id', value=idData.ids_df)
		#burden_table.to_csv(output_filepath, sep='\t')
	else:
		#section for future calls
		print("sorting method not supported")


def create_PHEWAS_file(annovar_filepath, output_filepath, restrictionList = None,lowerLimit = 0, higherLimit = None):
	dataset= pd.read_csv(annovar_filepath, sep="\t", header=None, names=["type of region", "genes", "chr", "start", "finish", "-1","-2","id","-3","-4","start2","finish2","color"])

	dataset["color"] = dataset.apply(kindOfVariant, axis=1)
	dataset = dataset.rename(columns = {"color":"Kind Of Variant"})
	cnv_dataset = dataset
	
	if not restrictionList == None:
		storage(restrictionList)
		#find boolean restriction
		dataset['on gene list?'] = dataset.apply(hasTheGene, axis=1)

		#dataframe with appropriate restrictions
		dataset2 = dataset[dataset['on gene list?']]
		del dataset2['on gene list?']
		dataset = dataset2
		cnv_dataset = dataset2	
	
	if not lowerLimit == 0 or not higherLimit == None:
		cnv_dataset = modifyDatasetWithLimits(dataset, lowerLimit, higherLimit)
					
	restrictedCNVs = dict()
	iDs = dict()

	#call to helper dataset
	for i in range(cnv_dataset.shape[0]):
		restrictedCNVs = updateRestrictedCNVs(i,cnv_dataset,restrictedCNVs)

	
	#create reference for dictionary CNV columns
	pheWasDict = {k: [] for k in restrictedCNVs.keys()}
	pheWasDict["id"] = []


	incr = -1
	for j in range(dataset.shape[0]):
		curr_id = dataset.iloc[j]["id"]
		if not curr_id in pheWasDict["id"]:
			#edit id column and dictionary for accurate insertion
			idList = pheWasDict["id"]
			idList.append(curr_id)
			pheWasDict["id"] = idList

			#update positioning for dictionary
			incr +=1
			#insertion
			iDs[curr_id]= incr

			#even out lists
			for pw_key, pw_value in pheWasDict.iteritems():
				if not pw_key == "id":
					pw_key_list = pw_value
					pw_key_list.append(0)
					pheWasDict[pw_key] = pw_key_list

		if curr_id in pheWasDict["id"]:
			#create key for comparison
			chromo = dataset.iloc[j]["chr"]
			start = dataset.iloc[j]["start"]
			finish = dataset.iloc[j]["finish"]
			kind = dataset.iloc[j]["Kind Of Variant"]

			dataset_key = createKey(chromo,start,finish,kind)

			#insert 1 at correct locations
			if dataset_key in pheWasDict:
				l = pheWasDict[dataset_key]
				l[iDs[curr_id]] = 1
				pheWasDict[dataset_key] = l


	#create DataFrame object
	pheWasTable = pd.DataFrame(pheWasDict)

	conversionDict("/Users/ashleynobi/Desktop/PythonWorkspace/long_id_short_id_conversion_table.txt", "reverse")

	#changes id to proper pheWas long id format
	pheWasTable["id"] = pheWasTable.apply(shortToLong, axis=1)

	#move id column to beginning
	idCol = pheWasTable.pop("id")

	pheWasTable.insert(loc=0, column='id', value=idCol)
	pheWasTable.to_csv(output_filepath, sep='\t')



def get_CNVs(filepath,output_filepath,restrictionList = None, lowerLimit = 0, higherLimit = None):
	dataset= pd.read_csv(filepath, sep="\t", header=None, names=["type of region", "genes", "chr", "start", "finish", "-1","-2","id","-3","-4","start2","finish2","color"])

	dataset["color"] = dataset.apply(kindOfVariant, axis=1)
	dataset = dataset.rename(columns = {"color":"Kind Of Variant"})
	dataset["length"] = dataset.apply(lambda row: (row["finish"] - row["start"]), axis=1)

	if not lowerLimit == 0 or not higherLimit == None:
		dataset = modifyDatasetWithLimits(dataset, lowerLimit, higherLimit)

	#dataset.to_csv("/Users/ashleynobi/Desktop/AFR_iCNV_raw_numerical_restriction.txt",sep='\t')
					
	if not restrictionList == None:
		storage(restrictionList)
		#find boolean restriction
		dataset['on gene list?'] = dataset.apply(hasTheGene, axis=1)

		#dataframe with appropriate restrictions
		final_dataset = dataset[dataset['on gene list?']]
		dataset = final_dataset


	dataset["entry"] = dataset.apply(lambda x: "chr" + str(x['chr']) + ":"  + str(x['start']) + "-" + str(x['finish']), axis=1)
	cnv_df = dataset[['id','entry','genes']].copy()
	cnv_df["target genes"] = dataset.apply(importantGenes, axis=1)
	
	cnv_df.sort_values(by=["id"])

	cnv_df.to_csv(output_filepath, sep='\t',index=False)

def convertChrToEntry(filepath,output_filepath):
	dataset= pd.read_csv(filepath, sep="\t", header=None, names=["id", "chr", "start", "finish", "color"])

	dataset["chr"] = dataset.apply(lambda x: str(x['chr']) + ":"  + str(x['start']) + "-" + str(x['finish']),axis=1)
	dataset = dataset.rename(columns = {"chr":"entry"})

	dataset["color"] = dataset.apply(kindOfVariant, axis=1)
	dataset = dataset.rename(columns = {"color":"Kind Of Variant"})

	fdataset = dataset[['id','entry','Kind Of Variant']].copy()
	fdataset.sort_values(by=["id"])
	fdataset.to_csv(output_filepath, sep='\t',index=False)

	








