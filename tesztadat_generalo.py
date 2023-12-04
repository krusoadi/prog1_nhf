import random
import unidecode
import os

if os.path.isdir("generated_testfiles") == False:
    os.mkdir("generated_testfiles")


for i in range(50):
    generalt_file = open(f"generated_testfiles\\tesztadat{i+1}.csv", "w")
    szint = 18
    szint_szoba = 16
    szobak = []
    szoba_adatok = []
    szoba_lakok = []

    for i in range(1, szint+1):
        for j in range(1, szint_szoba+1):
            szoba_adatok.append([])
            if j < 10:
                    szobak.append(f"{i}0{j}")
            else:
                szobak.append(f"{i}{j}")

    vezetek = ["Varga", "Molnar", "Kovacs", "Kiss", "Nagy", "Mezo", "Balazs", "Eros", "Horvath", "Ujhelyi", "Szabo", "Lengyel", "Gyori","Molnar", "Balogh", "Farkas","Lakatos", "Papp","Gal","Feher","Olah",]
    kereszt_nev = ["Petra", "Anna", "Lilla", "Dorina", "Eszter", "Reka", "Lili" ,"Adam", "Bela", "Marcell", "Zsolt", "Peter", "Zsigmond", "Egon", "Tamas", "Marton", "Andrea", "Lajos", "Ors", "Matyas", "Mihaly", "Zerind", "Anna", "Alexandra", "Kitti", "Bence", "Levente", "Botond", "Balint", "David", "Noel", "Robert", "Zoltan", "Agoston", "Barnabas", "Balazs"]
    szakok = ["v", "v", "v", "m","m","m","m","m","m","m","u"]

    with open("keresztnevek.txt", "r", encoding="UTF-8") as keresztnev_file:
        for i in range(500):
            nev = keresztnev_file.readline().strip()
            kereszt_nev.append(unidecode.unidecode(nev))
        

    with open("vezeteknevek.txt", "r", encoding="UTF-8") as vezeteknev_file:
        for i in range(67):
            nev = vezeteknev_file.readline().strip()
            vezetek.append(unidecode.unidecode(nev))
        

    azonosito = random.randint(1000,6000)

    for i in range(len(szobak)):
        szoba_lakok.append([])

        for j in range(4):
            szoba_lakok[i].append([])
        for j in range(4):
            azonosito += 1
            szoba_lakok[i][j].append(f"{random.choice(vezetek)} {random.choice(kereszt_nev)}")
            temp = random.choice(szakok)
            szoba_lakok[i][j].append(f"{azonosito}")
            szoba_lakok[i][j].append(f"{temp}")

        if i >= 5 * szint_szoba and i <= (9 * szint_szoba) - 1:
            szoba_adatok[i].append("I")
            for j in range(4):
                temp2 = round(random.randint(2,8)+random.random(), 2)
                atlagtemp = round(random.choice([1,1,2,2,2,2,3,3,3,3,3,4,4,4])+random.random(), 2)
                szoba_lakok[i][j].append("{:.2f}".format(temp2))
                szoba_lakok[i][j].append("{:.2f}".format(atlagtemp))
                bunti = random.choice([0,0,0,0,0,0,random.randint(1,19)])
                szoba_lakok[i][j].append(bunti)


        else:
            szoba_adatok[i].append("N")
            for j in range(4):
                temp2 = round(random.randint(2,8)+random.random(), 2)
                atlagtemp = round(random.choice([1,1,2,2,2,2,3,3,3,3,3,4,4,4])+random.random(), 2)
                szoba_lakok[i][j].append("{:.2f}".format(temp2))
                szoba_lakok[i][j].append("{:.2f}".format(atlagtemp))
                bunti = random.choice([0,0,0,0,0,0,random.randint(1,19)])
                szoba_lakok[i][j].append(bunti)


    for i in range(len(szobak)):
        generalt_file.write(f"{szobak[i]},")
        for j in range(len(szoba_lakok[i])):
                for k in range(len(szoba_lakok[i][j])):
                    generalt_file.write(f"{szoba_lakok[i][j][k]}")
                
                    if k != len(szoba_lakok[i][j]) - 1:
                        generalt_file.write(",")    
                
                if j != len(szoba_lakok[i]) - 1:
                    generalt_file.write(",")
                    

        generalt_file.write(f",{szoba_adatok[i][0]}")
        
        generalt_file.write("")
        generalt_file.write("\n")

    generalt_file.close()

    maxv = 0
    maxk = 0

    for i in range(len(vezetek)):
        if len(vezetek[i]) > maxv:
            maxv = len(vezetek[i])


    for i in range(len(kereszt_nev)):
        if len(kereszt_nev[i]) > maxk:
            maxk = len(kereszt_nev[i])

    print(maxv, maxk)