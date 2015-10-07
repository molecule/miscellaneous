import csv     # imports the csv module
import sys      # imports the sys module

firstRow = 1 
badgeId = 0xC8
dict = {'FirstName': 'Zara', 'LastName': 'Thustra', 
        0: 'x', 1: 'x', 2: 'x', 3: 'x', 4: 'x', 5: 'x', 6: 'x', 7: 'x', 8: 'x',
        9: 'x', 10: 'x', 11: 'x', 12: 'x', 13: 'x', 14: 'x', 15: 'x', 16: 'x',
        17: 'x'};
qPrompts = {0: '// Have you been to Texas before?', 
            1: '// Have you participated in a hackathon before?',
            2: '// Have you ever won an award?',
            3: '// Have you played with an Arduino or a microcontroller before this conference?',
            4: '// Do you play any musical instruments?',
            5: '// Is this your first time at GHC?',
            6: '// Are you a member of any professional organizations?',
            7: '// Did you get more than 7 hours of sleep last night',
            8: '// Are you the first person in your family to attend college?',
            9: '// Do you work with or study software engineering?',
            10: '// Do you work with or study hardware engineering?',
            11: '// Have you ever used a 3D printer?',
            12: '// Did you ever switch majors?',
            13: '// Have you ever been published?',
            14: '// Does your mobile phone use a Qualcomm chip?',
            15: '// Are you a robot?',
            16: '// Do you use a smartwatch?',
            17: '// Do you like to travel?'}

def write(userName):
    global badgeId
    name = userName + 'Survey.txt'
    print "Creating file: " + name
    try:
        file = open(name,'w')   # Trying to create a new file or open one
        file.write("// uint8_t badge_id_me = " + hex(badgeId) + 
                    "; // 7 bits " + str(badgeId) + "\n")
        file.write("// Serial.println(\"AT+DEVNAME=" + 
                    dict['FirstName'] + " " + dict['LastName'] + 
                    "\");\n")
        badgeId = badgeId - 1
        file.write("uint32_t myAnswers() {\n")
        file.write("    uint32_t myIRCode = 0;\n")
        file.write("    uint32_t Answer = 0;\n") 
        file.write("    // 1 for 'Yes' and a 0 for a 'No'\n")
        file.write("\n")
        for i in range(0, 18):
            file.write("    " + qPrompts[i] + "\n")
            file.write("    Answer = " +(str(dict[i]))+ ";\n")
            file.write("    myIRCode = myIRCode + (Answer<<"+ str(17-i)+");\n")
            file.write("\n")

        file.write("    return myIRCode;\n")
        file.write("}")    

        #file.write("%s\n" % str(row))
        file.close()
    finally:
        print "Created the file"


def parse():
    global firstRow
    if len(sys.argv) < 2:
        print "Usage: python python-parse-excel.py <path to file>"
        sys.exit(2)

    f = open(sys.argv[1], 'rU') # opens the csv file
    try:
        reader = csv.reader(f, dialect=csv.excel_tab)  # creates the reader object
        for row in reader:   # iterates the rows of the file in order
            if firstRow:
                firstRow = 0
            else:
                entries = row[0].split(',')  # gets each entry in the row.
                dict['FirstName'] = entries[2]
                dict['LastName'] = entries[3]
                print dict['FirstName'] + " " + dict['LastName']
                print len(entries)
                for i in range(0, 18):
                    dict[i] = entries[9+i]
                    print entries[i]
            while( not raw_input() ):
                break
            write(dict['FirstName'] + dict['LastName'])
        
    finally:
        f.close()      # closing

parse()