import sys, os


# may to august dates
dates = ["2022.05.30_", "2022.06.06_", "2022.06.13_", "2022.06.20_", "2022.06.27_", "2022.05.04_", "2022.05.11_", "2022.05.18_", "2022.05.25_"]

deltaTime = [200000, 240000]

if len(sys.argv) != 2:
	sys.stderr.write("Usage: get_rimouski_port_files.py directory \n")
	sys.exit(1)


directory = sys.argv[1]


allFilesAtGoodTime = []

for File in os.listdir(directory):
	#print(File)
	#print(File[11:17])
	try:
		time = int(File[11:17])
	except ValueError:
		continue;
	if time > deltaTime[0] and time < deltaTime[1]:
		allFilesAtGoodTime.append(File)



allFiles = []

for date in dates:

	threeFilePackage = []
	
	for File in allFilesAtGoodTime:
		#print(File)
		if date in File:
		
			threeFilePackage.append(File)
			
	allFiles.append(threeFilePackage)

#print(allFiles)

for package in allFiles:
	print(package)
