DROP TABLE Uzivatel CASCADE CONSTRAINTS;
DROP TABLE Tiket CASCADE CONSTRAINTS;
DROP TABLE Bug CASCADE CONSTRAINTS;
DROP TABLE Patch CASCADE CONSTRAINTS;
DROP TABLE Modul CASCADE CONSTRAINTS;
DROP TABLE Jazyk CASCADE CONSTRAINTS;
DROP TABLE Hrac CASCADE CONSTRAINTS;
DROP TABLE Programator CASCADE CONSTRAINTS;

DROP SEQUENCE ID_Uzivatel_S;
DROP SEQUENCE ID_Tiket_S;
DROP SEQUENCE ID_Bug_S;
DROP SEQUENCE ID_Patch_S;
DROP SEQUENCE ID_Modul_S;
DROP SEQUENCE ID_Jazyk_S;

CREATE SEQUENCE ID_Uzivatel_S;
CREATE SEQUENCE ID_Tiket_S;
CREATE SEQUENCE ID_Bug_S;
CREATE SEQUENCE ID_Patch_S;
CREATE SEQUENCE ID_Modul_S;
CREATE SEQUENCE ID_Jazyk_S;

CREATE TABLE Uzivatel
(
    ID_Uzivatel INT NOT NULL PRIMARY KEY,
    Login VARCHAR(20),
    Meno VARCHAR(40),
    email VARCHAR(40)
    CONSTRAINT CHK_Uzivatel CHECK(REGEXP_LIKE (
			email, '^[a-z]+[a-z0-9\.]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'
            ))
);

CREATE TABLE Hrac
(
    
    ID INT NOT NULL PRIMARY KEY,
    Login VARCHAR(20),
    Meno VARCHAR(40),
    email VARCHAR(40),
    CONSTRAINT Hrac_FK FOREIGN KEY (ID) REFERENCES Uzivatel(ID_Uzivatel)
    
);

CREATE TABLE Programator
(
    ID INT NOT NULL PRIMARY KEY,
    Login VARCHAR(20),
    Meno VARCHAR(40),
    email VARCHAR(40),
    CONSTRAINT Programator_FK FOREIGN KEY (ID) REFERENCES Uzivatel(ID_Uzivatel)
    
);
CREATE TABLE Tiket
(
    ID_Tiket INT NOT NULL PRIMARY KEY,
    Nazov VARCHAR(30),
    Datum_vytvorenia VARCHAR(10),
    Kategoria VARCHAR(20),
    Stav VARCHAR(10),
    ID_Uzivatel INT NULL REFERENCES Uzivatel (ID_Uzivatel)
);

CREATE TABLE Modul
(
    ID_Modul INT NOT NULL PRIMARY KEY,
    Nazov VARCHAR(20),
    Jazyk VARCHAR(20),
    ID_Uzivatel INT NULL REFERENCES Uzivatel (ID_Uzivatel)
);

CREATE TABLE Bug
(
    ID_Bug INT NOT NULL PRIMARY KEY,
    Typ VARCHAR(20),
    Popis VARCHAR(100),
    Zavaznost VARCHAR(20),
    ID_Tiket INT NULL REFERENCES Tiket (ID_Tiket),
    ID_Modul INT NULL REFERENCES Modul (ID_Modul)
);

CREATE TABLE Patch
(
    ID_Patch INT NOT NULL PRIMARY KEY,
    Nazov VARCHAR(20),
    Login VARCHAR(20),
    Datum_vydania DATE,
    Datum_zavedenia DATE,
    CONSTRAINT CHK_Tiket CHECK (Datum_vydania <= Datum_zavedenia),
    ID_Uzivatel INT NULL REFERENCES Uzivatel (ID_Uzivatel),
    ID_Bug INT NULL REFERENCES Bug (ID_Bug)
);

CREATE TABLE Jazyk
(
    ID_Jazyk INT NOT NULL PRIMARY KEY,
    Nazov VARCHAR(10)
);

/*Trigre na automaticke pisanie id*/

CREATE OR REPLACE TRIGGER TR_Uzivatel BEFORE INSERT ON Uzivatel FOR EACH ROW
begin
  if :NEW.ID_Uzivatel is null then 
    SELECT ID_Uzivatel_S.NEXTVAL INTO :NEW.ID_Uzivatel FROM DUAL; 
  end if; 
end TR_Uzivatel;
/

CREATE OR REPLACE TRIGGER TR_Tiket BEFORE INSERT ON Tiket FOR EACH ROW
begin
  if :NEW.ID_Tiket is null then 
    SELECT ID_Tiket_S.NEXTVAL INTO :NEW.ID_Tiket FROM DUAL; 
  end if; 
end TR_Tiket;
/

CREATE OR REPLACE TRIGGER TR_Bug BEFORE INSERT ON Bug FOR EACH ROW
begin
  if :NEW.ID_Bug is null then 
    SELECT ID_Bug_S.NEXTVAL INTO :NEW.ID_Bug FROM DUAL; 
  end if; 
end TR_Bug;
/

CREATE OR REPLACE TRIGGER TR_Patch BEFORE INSERT ON Patch FOR EACH ROW
begin
  if :NEW.ID_Patch is null then 
    SELECT ID_Patch_S.NEXTVAL INTO :NEW.ID_Patch FROM DUAL; 
  end if; 
end TR_Patch;
/

CREATE OR REPLACE TRIGGER TR_Modul BEFORE INSERT ON Modul FOR EACH ROW
begin
  if :NEW.ID_Modul is null then 
    SELECT ID_Modul_S.NEXTVAL INTO :NEW.ID_Modul FROM DUAL; 
  end if; 
end TR_Modul;
/

CREATE OR REPLACE TRIGGER TR_Jazyk BEFORE INSERT ON Jazyk FOR EACH ROW
begin
  if :NEW.ID_Jazyk is null then 
    SELECT ID_Jazyk_S.NEXTVAL INTO :NEW.ID_Jazyk FROM DUAL; 
  end if; 
end TR_Jazyk;
/



/*PLnenie tabuliek*/


INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Martin', 'Testovací', 'martin@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Winnie23', 'Winston', 'Winston@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Kai', 'Kaiden', 'Kaiden@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Ronnin', 'Ronan', 'Ronan@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Elsa', 'Ellis', 'Ellis@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Kyr5ten', 'Kyrsten', 'Kyrsten@gmail.com');
INSERT INTO Uzivatel VALUES (ID_Uzivatel_S.NEXTVAL, 'Martin98', 'Martin', 'martin@gmail.com');

INSERT INTO Tiket VALUES (ID_Tiket_S.NEXTVAL, 'Fusce mollis, id sagit et', TO_DATE('01-12-1997', 'dd-mm-yyyy'), 'Phasellu', 'V rieseni', '4');
INSERT INTO Tiket VALUES (ID_Tiket_S.NEXTVAL, 'Quisque et',  TO_DATE('01-12-1997', 'dd-mm-yyyy'), 'Vivamus', 'V rieseni', '5');
INSERT INTO Tiket VALUES (ID_Tiket_S.NEXTVAL, 'Nullam eue',  TO_DATE('01-12-1997', 'dd-mm-yyyy'), 'Phase', 'V rieseni', '2');
INSERT INTO Tiket VALUES (ID_Tiket_S.NEXTVAL, 'Sed vempe, sed tortor', TO_DATE('01-12-1997', 'dd-mm-yyyy'), 'In in ultr', 'Nezabrany', '2');
INSERT INTO Tiket VALUES (ID_Tiket_S.NEXTVAL, 'Sed pretiu', TO_DATE('01-12-1997', 'dd-mm-yyyy'), 'In in ult', 'Vyrieseny', '1');

INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'C');
INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'C#');
INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'JAVA');
INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'C++');
INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'HTML');
INSERT INTO Jazyk VALUES (ID_Jazyk_S.NEXTVAL, 'JavaScript');

INSERT INTO Patch VALUES (ID_Patch_S.NEXTVAL, 'Patch1120', 'Kai', TO_DATE('01-01-2020', 'dd-mm-yyyy'), TO_DATE('03-01-2020', 'dd-mm-yyyy'), '', '1');
INSERT INTO Patch VALUES (ID_Patch_S.NEXTVAL, 'Patch11220', 'Kyr5ten', TO_DATE('01-12-2020', 'dd-mm-yyyy'), TO_DATE('15-12-2020', 'dd-mm-yyyy'), '', '');
INSERT INTO Patch VALUES (ID_Patch_S.NEXTVAL, 'Patch301220', 'Kai', TO_DATE('30-12-2020', 'dd-mm-yyyy'), TO_DATE('30-01-2021', 'dd-mm-yyyy'), '', '');
INSERT INTO Patch VALUES (ID_Patch_S.NEXTVAL, 'Patch2321', 'Martin98', TO_DATE('02-03-2021', 'dd-mm-yyyy'), TO_DATE('02-03-2022', 'dd-mm-yyyy'), '', '');
INSERT INTO Patch VALUES (ID_Patch_S.NEXTVAL, 'Patch1422', 'Kyr5ten', TO_DATE('01-04-2022', 'dd-mm-yyyy'), TO_DATE('01-05-2022', 'dd-mm-yyyy'), '', '');

INSERT INTO Modul VALUES (ID_Modul_S.NEXTVAL, 'accumsan mauris', 'JAVA', '');
INSERT INTO Modul VALUES (ID_Modul_S.NEXTVAL, 'accumsan et', 'JAVA', '');
INSERT INTO Modul VALUES (ID_Modul_S.NEXTVAL, 'porta elementum', 'HTML', '');

INSERT INTO Bug VALUES (ID_Bug_S.NEXTVAL, 'UI', 'Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.', 'Lahka zavaznost', '', '1');
INSERT INTO Bug VALUES (ID_Bug_S.NEXTVAL, 'Commandline', 'Suspendisse quis malesuada lorem. In accumsan mauris a tempus faucibus.', 'Tazka zavaznost', '', '2');
INSERT INTO Bug VALUES (ID_Bug_S.NEXTVAL, 'Functional', 'Sed vel imperdiet augue, sed viverra tortor. Nullam vitae bibendum massa.', 'Lahka zavaznost', '', '3');
INSERT INTO Bug VALUES (ID_Bug_S.NEXTVAL, 'UI', 'Aenean at eros tempus turpis porta condimentum ut nec orci. ', 'Lahka zavaznost', '', '1');
INSERT INTO Bug VALUES (ID_Bug_S.NEXTVAL, 'Unit-level', 'Praesent commodo tempor lorem, eget porta nisl hendrerit ut.', 'Stredna zavaznost', '', '1');


-- vypise len uzivatelov, ktori vytvorili patch po datume 1.1.2021
SELECT Login, Email FROM Uzivatel WHERE NOT EXISTS (SELECT * FROM Patch WHERE Login = Uzivatel.Login AND Datum_vydania <= TO_DATE('01-01-2021', 'dd-mm-yyyy'));

--vypise vsetky tikety, ktore vytvoril uzivatel s loginom "Winnie23"
SELECT Nazov, Login, Datum_vytvorenia FROM Tiket NATURAL JOIN Uzivatel WHERE Login = 'Winnie23';

--vypise pocet patchov, ktore vydal dany uzivatel za rok 2020
SELECT Login, COUNT(*) pocet_patchov FROM Patch WHERE datum_vydania BETWEEN TO_DATE('01-01-2020', 'dd-mm-yyyy') AND TO_DATE('31-12-2020', 'dd-mm-yyyy') GROUP BY (Login);

--vypise len moduly, ktore obsahuju bug s lahkou zavaznostou a ich pocet
SELECT Nazov, COUNT(*) Pocet_bugov FROM Modul NATURAL JOIN Bug WHERE Zavaznost = 'Lahka zavaznost' GROUP BY (Nazov);

-- vypise nazov a stav tiketu ktory obsahuje dany typ bugu a bol vytvoreny danym uzivatelom
SELECT Nazov, Stav FROM Tiket WHERE EXISTS (SELECT * FROM Uzivatel NATURAL JOIN Bug WHERE Typ = 'UI' AND Email = 'Winston@gmail.com');

--vypise moduly v ktorych sa nachadza bug s lahkou zavaznostou
SELECT ID_Modul, Nazov, Jazyk FROM Modul WHERE Nazov IN (SELECT Nazov FROM Bug WHERE Zavaznost = 'Lahka zavaznost');




/*
-- Pristupy k tabulkam
GRANT ALL PRIVILEGES ON Uzivatel TO xhorva14;
GRANT SELECT ON Programator TO xhorva14;
GRANT ALL PRIVILEGES ON Bug TO xhorva14;
GRANT ALL PRIVILEGES ON Patch TO xhorva14;
GRANT ALL PRIVILEGES ON Tiket TO xhorva14;
GRANT ALL PRIVILEGES ON Modul TO xhorva14;
GRANT ALL PRIVILEGES ON Jazyk TO xhorva14;
*/










CREATE OR REPLACE PROCEDURE odstran_stare (datum Tiket.Datum_vytvorenia%TYPE) AS
CURSOR v_cur IS
SELECT Stav, Datum_vytvorenia
FROM Tiket;
radek v_cur%ROWTYPE;
BEGIN
  OPEN v_cur;
  FETCH v_cur INTO radek;  
  WHILE v_cur%found LOOP
    IF radek.Datum_vytvorenia < datum THEN
      DELETE FROM Tiket WHERE radek.Stav = 'Vyrieseny';
    END IF;
    FETCH v_cur INTO radek;
  END LOOP; 
  CLOSE v_cur;
END odstran_stare;
/




/*CREATE OR REPLACE PROCEDURE odstran_solved (solved_bug Patch.ID_Patch%TYPE) AS
CURSOR v_cur IS
SELECT ID_Bug, Stav
FROM Patch;
radek v_cur%ROWTYPE;
BEGIN
  OPEN v_cur;
  FETCH v_cur INTO radek;  
  WHILE v_cur%found LOOP
      DELETE FROM Bug WHERE radek.ID_Bug = solved_bug.ID_Bug AND radek.Stav = 'Vyrieseny';
    FETCH v_cur INTO radek;
  END LOOP; 
  CLOSE v_cur;
END odstran_solved;
/
*/




