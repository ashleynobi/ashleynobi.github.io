#!/usr/bin/python

import pandas
import annovar_modifications
from Tkinter import *
import tkFileDialog
import Tkinter as tk



class MainView(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.title("ANNOVAR MODIFICATIONS")

        # the container is where we'll stack a bunch of frames
        # on top of each other, then the one we want visible
        # will be raised above the others
        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}
        for F in (StartPage, LoadPage, RunPage):
            page_name = F.__name__
            frame = F(parent=container, controller=self)
            self.frames[page_name] = frame

            # put all of the pages in the same location;
            # the one on the top of the stacking order
            # will be the one that is visible.
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame("StartPage")

    def show_frame(self, page_name):
        frame = self.frames[page_name]
        frame.tkraise()

class StartPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self, parent)
		self.controller = controller

		optionbutton = Button(self, text="Load Information", command=lambda: controller.show_frame("LoadPage"))
		optionbutton.pack()
		optionbutton.place(bordermode=OUTSIDE,relwidth=1, relheight=0.5,relx = 0, rely = 0)

		optionbutton2 = Button(self, text="Run Scripts", command=lambda: controller.show_frame("RunPage"))
		optionbutton2.pack()
		optionbutton2.place(bordermode=OUTSIDE,relwidth=1, relheight=0.5,relx = 0,rely = 0.5)

class LoadPage(tk.Frame):
	annovar_filepath = None
	restrictionList = None
	lowerLimit = 0
	higherLimit = None
	def __init__(self, parent, controller):
		tk.Frame.__init__(self, parent)
		self.controller = controller

		topframe = Frame(self, height = 80, width = 250 )
		topframe.pack(side = TOP)

		midframe = Frame(self, height = 80, width = 250 )
		midframe.pack(side = TOP)

		bottomframe = Frame(self, height = 90, width = 250 )
		bottomframe.pack(side = BOTTOM)

		bottomframel = Frame(bottomframe, height = 90, width = 125)
		bottomframel.pack(side = LEFT)

		bottomframer = Frame(bottomframe, height = 90, width = 125)
		bottomframer.pack(side = RIGHT)

		goback = Button(topframe, text="Go Back", command=lambda: controller.show_frame("StartPage"))
		goback.pack()
		goback.place(bordermode=OUTSIDE, relheight = 1, relwidth = 1, relx = 0, rely = 0)

		filebutton = Button(midframe, text="Load ANNOVAR File", command = self.openAnnovarFile)
		filebutton.pack()
		filebutton.place(bordermode=OUTSIDE, relheight = 0.5, relwidth = 1, relx = 0, rely = 0)

		filebutton2 = Button(midframe, text="Load Restriction File", command =  self.openRestrictionFile)
		filebutton2.pack()
		filebutton2.place(bordermode=OUTSIDE, relheight = 0.5, relwidth = 1, relx = 0, rely = 0.5)

		entrybutton = Button(bottomframel, text="Enter", command = self.checkEntry)
		entrybutton.pack(side = BOTTOM)
		self.entry = Entry(bottomframel)
		self.entry.pack(side = BOTTOM)
		label = Label(bottomframel, text = "Lower Restriction #")
		label.pack(side = BOTTOM)

		entrybutton2 = Button(bottomframer, text="Enter",command = self.checkEntry2)
		entrybutton2.pack(side = BOTTOM)
		self.entry2 = Entry(bottomframer)
		self.entry2.pack(side = BOTTOM)
		label2 = Label(bottomframer, text = "Higher Restriction #")
		label2.pack(side = BOTTOM)

	def openAnnovarFile(self):
		self.annovar_filepath = tkFileDialog.askopenfilename()
		LoadPage.annovar_filepath = self.annovar_filepath

	def openRestrictionFile(self):
		self.restrictionList = tkFileDialog.askopenfilename()
		LoadPage.restrictionList = self.annovar_filepath

	def checkEntry(self):
		self.lowerLimit = self.entry.get()
		LoadPage.lowerLimit = self.lowerLimit
		self.entry.delete(0, END)

	def checkEntry2(self):
		self.higherLimit = self.entry2.get()
		LoadPage.higherLimit = self.higherLimit
		self.entry2.delete(0, END)


class RunPage(tk.Frame):
	output_filepath = None
	output_filepath2 = None
	def __init__(self, parent, controller):
		tk.Frame.__init__(self, parent)
		self.controller = controller

		#topframe = Frame(self, height = 75, width = 250)
		topframe = Frame(self)
		topframe.pack(side="top", fill="both", expand=True)
		#topframe.pack(side = TOP)

		#midframe = Frame(self, height = 175, width = 250 )
		midframe = Frame(self)
		midframe.pack(side="top", fill="both", expand=True)
		#midframe.pack(side = TOP)

		goback = Button(topframe, text="Go Back", command=lambda: controller.show_frame("StartPage"))
		goback.pack()
		goback.place(bordermode=OUTSIDE,relwidth=1, relheight=0.5,relx = 0, rely = 0)

		entrybutton = Button(topframe, text="Enter",command = self.checkEntry)
		entrybutton.pack(side = BOTTOM)
		self.entry = Entry(topframe)
		self.entry.pack(side = BOTTOM)
		label = Label(topframe, text = "Enter Output Filepath")
		label.pack(side = BOTTOM)

		optionbutton = Button(midframe, text="Find Genes in Restriction", command = self.find_genes_in_restriction)
		optionbutton.pack()
		optionbutton.place(bordermode=OUTSIDE,relwidth=1, relheight=0.2,relx = 0, rely = 0)

		optionbutton2 = Button(midframe, text="Create Track for Genes in Restriction", command = self.trackForGenesInRestriction)
		optionbutton2.pack()
		optionbutton2.place(bordermode=OUTSIDE,relwidth=1, relheight=0.2,relx = 0, rely = 0.2)

		optionbutton3 = Button(midframe, text="Create Burden Table", command = self.load_for_burden_tables)
		optionbutton3.pack()
		optionbutton3.place(bordermode=OUTSIDE,relwidth=1, relheight=0.2,relx = 0, rely = 0.4)

		optionbutton4 = Button(midframe, text="Create PheWAS", command = self.create_PHEWAS_file)
		optionbutton4.pack()
		optionbutton4.place(bordermode=OUTSIDE,relwidth=1, relheight=0.2,relx = 0, rely = 0.6)

		optionbutton5 = Button(midframe, text="Get CNVs", command = self.getCNVs)
		optionbutton5.pack()
		optionbutton5.place(bordermode=OUTSIDE,relwidth=1, relheight=0.2,relx = 0, rely = 0.8)


	def checkEntry(self):
		self.output_filepath = self.entry.get()
		RunPage.output_filepath = self.output_filepath
		print(self.output_filepath) 
		self.entry.delete(0, END)

	def find_genes_in_restriction(self):
		if LoadPage.annovar_filepath == None:
			self.LoadAnnovarError()
		elif self.output_filepath == None:
			self.LoadOutputError()
		else:
			try:
				output_filepath2 = output_filepath.replace(".txt","NEG.txt")
				a.find_genes_in_restriction(LoadPage.annovar_filepath,LoadPage.restrictionList, output_filepath, output_filepath2, LoadPage.lowerLimit, LoadPage.higherLimit)
			except:
				self.loadRunError() 

	def trackForGenesInRestriction(self):
		if LoadPage.annovar_filepath == None:
			self.LoadAnnovarError()
		elif self.output_filepath == None:
			self.LoadOutputError()
		else:
			print(LoadPage.annovar_filepath)
		#a.trackForGenesInRestriction(LoadPage.annovar_filepath,LoadPage.restrictionList, output_filepath,LoadPage.lowerLimit, LoadPage.higherLimit)
	
	def load_for_burden_tables(self):
		if LoadPage.annovar_filepath == None:
			self.LoadAnnovarError()
		elif self.output_filepath == None:
			self.LoadOutputError()
		else:
			print(LoadPage.annovar_filepath)
		#a.load_for_burden_tables(LoadPage.annovar_filepath,output_filepath,sort,LoadPage.restrictionList, LoadPage.lowerLimit, LoadPage.higherLimit)

	def create_PHEWAS_file(self):
		if LoadPage.annovar_filepath == None:
			self.LoadAnnovarError()
		elif self.output_filepath == None:
			self.LoadOutputError()
		else:
			print(LoadPage.annovar_filepath)
		#a.create_PHEWAS_file(LoadPage.annovar_filepath, output_filepath, LoadPage.restrictionList,LoadPage.lowerLimit, LoadPage.higherLimit)

	def getCNVs(self):
		if LoadPage.annovar_filepath == None:
			self.LoadAnnovarError()
		elif self.output_filepath == None:
			self.LoadOutputError()
		else:
			print(LoadPage.annovar_filepath)
		#a.getCNVs(LoadPage.annovar_filepath,LoadPage.restrictionList, output_filepath, LoadPage.lowerLimit, LoadPage.higherLimit)

	def LoadAnnovarError(self):
		toplevel = Toplevel()
		l = Label(toplevel, text="Enter Annovar File!", height = 0, width = 30)
		l.pack()

	def LoadOutputError(self):
		toplevel = Toplevel()
		l = Label(toplevel, text="Enter Output File!", height = 0, width = 30)
		l.pack()

	def LoadRunError(self):
		toplevel = Toplevel()
		l = Label(toplevel, text="There was a problem running this script, try again!", height = 0, width = 50)
		l.pack()

if __name__ == "__main__":
	main = MainView()
	main.mainloop()
