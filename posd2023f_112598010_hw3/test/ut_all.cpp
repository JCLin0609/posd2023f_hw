#include <gtest/gtest.h>

#include "ut_file.h"
#include "ut_folder.h"
#include "ut_node.h"
#include "ut_visitor.h"
#include "ut_iterator.h"

#include "ut_scanner.h"
#include "ut_parser.h"
#include "ut_builder.h"
#include "ut_tree_visitor.h"

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
