#include <gtest/gtest.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include "../src/shape.h"
#include "../src/triangle.h"
#include "../src/compound.h"
#include "../src/unit_of_work.h"
#include "../src/drawing_mapper.h"
#include "../src/painter_mapper.h"
#include "../src/drawing.h"

class DBSuite : public ::testing::Test
{
protected:
    void SetUp() override
    {
        create_drawing_table();
        create_painter_table();
        populate_drawings();
        populate_painters();
        dm = DrawingMapper::instance();
        dm->setDB("resource/drawing.db");
        pm = PainterMapper::instance();
        pm->setDB("resource/painter.db");
    }

    void TearDown() override
    {
        drop_drawing_table();
        drop_painter_table();
        sqlite3_close(db);
        sqlite3_close(db_p);
        dm->closeDB();
        pm->closeDB();
    }

    void create_drawing_table()
    {
        ASSERT_EQ(0, sqlite3_open("resource/drawing.db", &db));
        const char *stmt = "CREATE TABLE drawing ("
                           "ID         char(6) PRIMARY KEY     not null,"
                           "painter    varchar(50)             not null,"
                           "shapes varchar(1028),"
                           "FOREIGN KEY(painter) REFERENCES painter(ID))";
        int rc = sqlite3_exec(db, stmt, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
    }

    void drop_drawing_table()
    {
        const char *dropStmt = "DROP TABLE drawing";
        int rc = sqlite3_exec(db, dropStmt, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
    }

    void create_painter_table()
    {
        ASSERT_EQ(0, sqlite3_open("resource/painter.db", &db_p));
        const char *stmt = "CREATE TABLE painter ("
                           "ID         char(6) PRIMARY KEY     not null,"
                           "name    varchar(50)             not null)";
        int rc = sqlite3_exec(db_p, stmt, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
    }

    void drop_painter_table()
    {
        const char *dropStmt = "DROP TABLE painter";
        int rc = sqlite3_exec(db_p, dropStmt, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
    }

    void populate_drawings()
    {
        const char *s1 = "INSERT INTO drawing"
                         "(ID, painter, shapes)"
                         "values"
                         "('d_0001', 'p_0001', 'triangle 1 1 1')";
        int rc = sqlite3_exec(db, s1, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
        ASSERT_EQ(SQLITE_OK, rc);

        const char *s2 = "INSERT INTO drawing"
                         "(ID,  painter,   shapes)"
                         "values"
                         "('d_0002', 'p_0001', 'triangle 2 2 2')";
        rc = sqlite3_exec(db, s2, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
        ASSERT_EQ(SQLITE_OK, rc);

        const char *s3 = "INSERT INTO drawing"
                         "(ID,  painter,   shapes)"
                         "values"
                         "('d_0003', 'p_0002', 'compound { triangle 1 1 1 triangle 2 2 2 } triangle 3 3 3')";
        rc = sqlite3_exec(db, s3, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
        ASSERT_EQ(SQLITE_OK, rc);
    }

    void populate_painters()
    {
        const char *s1 = "INSERT INTO painter"
                         "(ID, name)"
                         "values"
                         "('p_0001', 'Patrick')";
        int rc = sqlite3_exec(db_p, s1, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
        ASSERT_EQ(SQLITE_OK, rc);

        const char *s2 = "INSERT INTO painter"
                         "(ID, name)"
                         "values"
                         "('p_0002', 'Mary')";
        rc = sqlite3_exec(db_p, s2, NULL, NULL, &err_msg);
        display_err_msg_and_fail_if_any(rc);
        ASSERT_EQ(SQLITE_OK, rc);
    }

    void display_err_msg_and_fail_if_any(int rc)
    {
        if (rc)
        {
            std::cout << "db error: " << err_msg << std::endl;
            sqlite3_free(err_msg);
        }
        ASSERT_EQ(SQLITE_OK, rc);
    }

    sqlite3 *db;
    sqlite3 *db_p;
    char *err_msg = nullptr;
    DrawingMapper *dm;
    PainterMapper *pm;
};

TEST_F(DBSuite, Sanity)
{    
}

TEST_F(DBSuite, findDrawing)
{
    Drawing *drawing = dm->find("d_0001");

    EXPECT_TRUE(UnitOfWork::instance()->inClean("d_0001"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("d_0001"));
    EXPECT_TRUE(UnitOfWork::instance()->inClean("p_0001"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("p_0001"));
    ASSERT_EQ(drawing->id(), "d_0001");
    ASSERT_EQ(drawing->getShape(0)->perimeter(), 3);
    ASSERT_EQ(drawing->painter()->id(), "p_0001");
    ASSERT_EQ(drawing->painter()->name(), "Patrick");
}

TEST_F(DBSuite, findDrawingWithCompoundShape)
{
    Drawing *drawing = dm->find("d_0003");

    ASSERT_EQ(drawing->id(), "d_0003");
    ASSERT_EQ(drawing->getShape(0)->perimeter(), 9);
    ASSERT_EQ(drawing->getShape(1)->perimeter(), 9);
    ASSERT_EQ(drawing->painter()->id(), "p_0002");
    ASSERT_EQ(drawing->painter()->name(), "Mary");
}

TEST_F(DBSuite, addDrawing)
{
    std::list<Shape *> shape = {new Triangle(1, 1, 1)};
    Drawing *drawing = new Drawing("d_0004", pm->find("p_0001"), shape);
    dm->add(drawing);

    UnitOfWork::instance()->commit();
    Drawing *d = dm->find("d_0004");
    ASSERT_EQ(d->id(), "d_0004");
    ASSERT_EQ(d->getShape(0)->perimeter(), 3);
    ASSERT_EQ(d->painter()->id(), "p_0001");
    ASSERT_EQ(d->painter()->name(), "Patrick");
}

TEST_F( DBSuite, deleteDrawing )
{
    Drawing *drawing = dm->find("d_0001");
    dm->del(drawing->id());

    Drawing *d = dm->find("d_0001");
    ASSERT_EQ(nullptr, d);
}

TEST_F(DBSuite, updateDrawing)
{
    Drawing *drawing = dm->find("d_0001");
    dm->update(drawing->id());

    ASSERT_EQ(drawing->id(), "d_0001");
    ASSERT_EQ(drawing->getShape(0)->perimeter(), 3);
    ASSERT_EQ(drawing->painter()->id(), "p_0001");
    ASSERT_EQ(drawing->painter()->name(), "Patrick");
}

TEST_F(DBSuite, updateDrawingWithCompoundShape)
{
    Drawing *drawing = dm->find("d_0003");
    dm->update(drawing->id());

    ASSERT_EQ(drawing->id(), "d_0003");
    ASSERT_EQ(drawing->getShape(0)->perimeter(), 9);
    ASSERT_EQ(drawing->getShape(1)->perimeter(), 9);
    ASSERT_EQ(drawing->painter()->id(), "p_0002");
    ASSERT_EQ(drawing->painter()->name(), "Mary");
}

TEST_F(DBSuite, findPainter)
{
    Painter *painter = pm->find("p_0001");

    EXPECT_TRUE(UnitOfWork::instance()->inClean("p_0001"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("p_0001"));
    ASSERT_EQ(painter->id(), "p_0001");
    ASSERT_EQ(painter->name(), "Patrick");
}

TEST_F(DBSuite, addPainter)
{
    Painter *painter = new Painter("p_0003", "John");
    pm->add(painter);

    Painter *p = pm->find("p_0003");
    ASSERT_EQ(p->id(), "p_0003");
    ASSERT_EQ(p->name(), "John");
}

TEST_F(DBSuite, deletePainter)
{
    Painter *painter = pm->find("p_0001");
    pm->del(painter->id());

    UnitOfWork::instance()->commit();
    Painter *p = pm->find("p_0001");
    ASSERT_EQ(nullptr, p);
}

TEST_F(DBSuite, updatePainter)
{
    Painter *painter = pm->find("p_0001");
    pm->update(painter->id());

    Painter *p = pm->find("p_0001");
    ASSERT_EQ(p->id(), "p_0001");
    ASSERT_EQ(p->name(), "Patrick");
}

TEST_F(DBSuite, commitPainter)
{
    Painter *painter = pm->find("p_0001");
    painter->setName("John");
    UnitOfWork::instance()->commit();

    EXPECT_TRUE(UnitOfWork::instance()->inClean("p_0001"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("p_0001"));
    ASSERT_EQ(painter->id(), "p_0001");
    ASSERT_EQ(painter->name(), "John");
}

TEST_F(DBSuite, commitDrawing)
{
    Drawing *drawing = dm->find("d_0001");
    drawing->setPainter(pm->find("p_0002"));
    UnitOfWork::instance()->commit();

    EXPECT_TRUE(UnitOfWork::instance()->inClean("d_0001"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("d_0001"));
    EXPECT_TRUE(UnitOfWork::instance()->inClean("p_0002"));
    EXPECT_FALSE(UnitOfWork::instance()->inDirty("p_0002"));
    ASSERT_EQ(drawing->id(), "d_0001");
    ASSERT_EQ(drawing->getShape(0)->perimeter(), 3);
    ASSERT_EQ(drawing->painter()->id(), "p_0002");
    ASSERT_EQ(drawing->painter()->name(), "Mary");
}

