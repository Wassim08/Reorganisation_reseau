CC=gcc -Wall
SRC = ./src/
HEA = ./headers/
PRE = ./precomp/
BIN = ./bin/
src = $(wildcard $(SRC)*.c)
allprecomp = $(src:./src/%.c=./precomp/%.o)

ifdef OS
	EXT=.exe
	CLEAN=rmdir /s /q
else
	CLEAN=rm -r 
endif
ALL = $(BIN)ChaineMain$(EXT) $(BIN)ChaineTest$(EXT) $(BIN)ReconstitueReseau$(EXT)  $(BIN)generationAleatoireFileMain$(EXT) $(BIN)generationAleatoireFileMain2$(EXT) $(BIN)PerformanceReconstitution$(EXT) $(BIN)PerformanceReconstitution2$(EXT)   $(BIN)PerformanceReorganisation$(EXT) $(BIN)PerformanceTableHachage$(EXT) $(BIN)Test$(EXT)


ALL :	  $(ALL)


$(BIN)%$(EXT):  |bin
	$(CC) $^ -o $(basename $@) -lm

$(PRE)%.o: $(SRC)%.c | precomp
	$(CC) -c $< -o $@ -g


$(PRE)Struct_Liste.o: $(HEA)Struct_Liste.h
Struct_Liste = $(PRE)Struct_Liste.o

$(PRE)Struct_File.o: $(HEA)Struct_File.h
Struct_File = $(PRE)Struct_File.o


$(PRE)Chaine.o:  $(HEA)Chaine.h $(HEA)SVGwriter.h $(HEA)OutilT.h
Chaine = $(PRE)Chaine.o $(SVGwriter) $(OutilT)

$(PRE)OutilT.o : $(HEA)OutilT.h
OutilT = $(PRE)OutilT.o

$(PRE)ChaineMain.o: $(HEA)Chaine.h $(HEA)OutilT.h
ChaineMain = $(PRE)ChaineMain.o $(Chaine) $(OutilT) 

$(PRE)ChaineTest.o: $(HEA)Chaine.h
ChaineTest =$(PRE)ChaineTest.o $(Chaine)


$(PRE)Reseau.o: $(HEA)Reseau.h  $(HEA)Chaine.h $(HEA)SVGwriter.h
Reseau = $(PRE)Reseau.o $(Chaine) $(SVGwriter)

$(PRE)SVGwriter.o: $(HEA)SVGwriter.h
SVGwriter = $(PRE)SVGwriter.o

$(PRE)Hachage.o: $(HEA)Hachage.h $(HEA)Reseau.h $(HEA)Chaine.h
Hachage = $(PRE)Hachage.o $(Reseau) $(Chaine)

$(PRE)GrapheMain.o : $(HEA)Graphe.h $(HEA)Reseau.h $(HEA)Chaine.h $(HEA)OutilT.h $(HEA)Struct_File.h $(HEA)Struct_Liste.h
GrapheMain = $(PRE)GrapheMain.o $(Graphe) $(Reseau) $(Chaine) $(OutilT) $(Struct_File) $(Struct_Liste)

$(PRE)ArbreQuat.o: $(HEA)ArbreQuat.h $(HEA)Reseau.h $(HEA)Chaine.h
ArbreQuat = $(PRE)ArbreQuat.o $(Reseau) $(Chaine)


$(PRE)ReconstitueReseau.o : $(HEA)Reseau.h $(HEA)Hachage.h $(HEA)ArbreQuat.h $(HEA)OutilT.h
ReconstitueReseau = $(PRE)ReconstitueReseau.o $(Reseau) $(Hachage) $(ArbreQuat) $(OutilT)

$(PRE)PerformanceReconstitution.o : $(HEA)Reseau.h $(HEA)Hachage.h $(HEA)ArbreQuat.h $(HEA)OutilT.h $(HEA)Chaine.h
PerformanceReconstitution = $(PRE)PerformanceReconstitution.o $(Reseau) $(Hachage) $(ArbreQuat) $(OutilT) $(Chaine)

$(PRE)PerformanceReconstitution2.o : $(HEA)Reseau.h $(HEA)Hachage.h $(HEA)ArbreQuat.h $(HEA)OutilT.h $(HEA)Chaine.h
PerformanceReconstitution2 = $(PRE)PerformanceReconstitution2.o $(Reseau) $(Hachage) $(ArbreQuat) $(OutilT) $(Chaine)

$(PRE)PerformanceReorganisation.o : $(HEA)Reseau.h  $(HEA)Chaine.h $(HEA)Graphe.h
PerformanceReorganisation = $(PRE)PerformanceReorganisation.o $(Reseau) $(Chaine) $(Graphe)

$(PRE)generationAleatoireFileMain2.o : $(HEA)Chaine.h
generationAleatoireFileMain2 = $(PRE)generationAleatoireFileMain2.o $(Chaine)

$(PRE)generationAleatoireFileMain.o: $(HEA)Chaine.h
generationAleatoireFileMain$ = $(PRE)generationAleatoireFileMain.o $(Chaine)

$(PRE)Graphe.o : $(HEA)Graphe.h $(HEA)Struct_Liste.h $(HEA)Reseau.h $(HEA)Struct_File.h $(HEA)Hachage.h $(HEA)ArbreQuat.h
Graphe =  $(PRE)Graphe.o $(Struct_Liste) $(Reseau) $(Struct_File) $(Hachage) $(ArbreQuat)

$(PRE)PerformanceTableHachage.o : $(HEA)OutilT.h $(HEA)Reseau.h  $(HEA)Chaine.h
PerformanceTableHachage = $(PRE)PerformanceTableHachage.o   $(OutilT) $(Reseau)  $(Hachage)  $(Chaine)

$(PRE)Test.o : $(HEA)Graphe.h $(HEA)Struct_Liste.h $(HEA)Reseau.h $(HEA)Struct_File.h $(HEA)Hachage.h $(HEA)ArbreQuat.h
Test =  $(PRE)Test.o $(Struct_Liste) $(Reseau) $(Struct_File) $(Hachage) $(ArbreQuat)


$(BIN)ChaineTest$(EXT): $(ChaineTest)
$(BIN)ChaineMain$(EXT):$(ChaineMain) 
$(BIN)ReconstitueReseau$(EXT): $(ReconstitueReseau)
$(BIN)PerformanceReconstitution$(EXT) : $(PerformanceReconstitution)
$(BIN)PerformanceReconstitution2$(EXT) : $(PerformanceReconstitution2)
$(BIN)generationAleatoireFileMain$(EXT) :  $(generationAleatoireFileMain)
$(BIN)generationAleatoireFileMain2$(EXT) :$(generationAleatoireFileMain2)
$(BIN)PerformanceReorganisation$(EXT) : $(PerformanceReorganisation)
$(BIN)PerformanceTableHachage$(EXT) :$(PerformanceTableHachage)
$(BIN)GrapheMain$(EXT): $(GrapheMain)
$(BIN)Test$(EXT): $(Test)



precomp :
	mkdir precomp
bin :
	mkdir bin

clean :
	$(CLEAN) precomp

cleanALL :
	$(CLEAN) precomp bin
