
------------------------------------------------------------------
-- base sql
------------------------------------------------------------------
-- create table
CREATE TABLE TestTable
(
    ID int not null,
    TestName varchar(255) not null,
    TestVal int not null,
    PRIMARY KEY (ID)
)

-- select
SELECT ID FROM TestTable WHERE TestVal in (1, 2, 3, 4)
select ID from TestTable where TestVal >0 order by TestVal desc

-- UPDATE
update TestTable set TestVal = 0 where ID = 100

-- DELETE
DELETE from TestTable where TestVal > 0

-- INSERT
insert into TestTable (ID, TestName, TestVal) VALUE (1, "Test", 10)

------------------------------------------------------------------

