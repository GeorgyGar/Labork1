#include "tbitfield.h"

#include <gtest.h>

// test fixture
// https://github.com/google/googletest/blob/master/docs/primer.md#test-fixtures-using-the-same-data-configuration-for-multiple-tests-same-data-multiple-tests

class BigBitFields : public ::testing::Test {
protected:
  TBitField bf1;
  TBitField bf2;

public:
	//void SetUp() {}
  BigBitFields() : bf1(50), bf2(100) {
    bf1.SetBit(1);
    bf1.SetBit(33);
    bf1.SetBit(49);

    bf2.SetBit(1);
    bf2.SetBit(49);
    bf2.SetBit(99);
  }

  ~BigBitFields() {} // TearDown()
};

TEST_F(BigBitFields, or_operator_for_bitfields_with_different_memlen_bigger_size_right)
{
	// BigBitFields()
  TBitField res(100);
  res.SetBit(1);
  res.SetBit(49);

  EXPECT_EQ(res, bf1 & bf2);
  //  ~BigBitFields()
}

TEST_F(BigBitFields, or_operator_for_bitfields_with_different_memlen_bigger_size_left)
{
  TBitField res(100);
  res.SetBit(1);
  res.SetBit(33);
  res.SetBit(49);
  res.SetBit(99);

  EXPECT_EQ(res, bf2 | bf1);
}

TEST_F(BigBitFields, and_operator_for_bitfields_with_different_memlen_bigger_size_left)
{
	TBitField res(100);
	res.SetBit(1);
	res.SetBit(33);
	res.SetBit(49);
	res.SetBit(99);

	EXPECT_EQ(res, bf2 | bf1);
}
//
//// -----------------------------------------------------------------------------------------------------------
//// Value parameterized test
//// https://github.com/google/googletest/blob/master/docs/advanced.md#value-parameterized-tests
//
class ParameterizedBitFields : public ::testing::TestWithParam<int>
{
protected:
  TBitField bf1;

public:
  ParameterizedBitFields(): bf1(GetParam()) 
  {
    for (int i = 0; i < GetParam(); i += 10)
      bf1.SetBit(i);
  }

  ~ParameterizedBitFields() {}
};

TEST_P(ParameterizedBitFields, can_create_bitfield)
{
  EXPECT_EQ(GetParam(), bf1.GetLength());
}

TEST_P(ParameterizedBitFields, inverse_is_correct)
{
  int maxSize = GetParam() + 32 - GetParam() % 32;
  TBitField res(maxSize);
  res = res | (~bf1);

  EXPECT_EQ(0, res.GetBit(0));
  EXPECT_EQ(1, res.GetBit(GetParam() - 1));
  EXPECT_EQ(0, res.GetBit(GetParam()));
  EXPECT_EQ(0, res.GetBit(maxSize - 1));
}



INSTANTIATE_TEST_CASE_P(Instantiation1,
  ParameterizedBitFields,
  ::testing::Values(20, 100, 1000));

int test_values[] = { 30, 300 };

INSTANTIATE_TEST_CASE_P(Instantiation2,
  ParameterizedBitFields,
  ::testing::ValuesIn(test_values));


//--------------------------------------------------------
class ParameterizedBitFieldsSimple : public ::testing::TestWithParam<int>
{
protected:
	TBitField bf1;
	TBitField bf2;

public:
	ParameterizedBitFieldsSimple() : bf1(GetParam()), bf2(GetParam())
	{
		for (int i = 0; i < GetParam(); i += 10)
			bf1.SetBit(i);
		bf2.SetBit(0);
		bf2.SetBit(1);
	}

	int ZeroTest(TBitField& bf, int k)
	{
		for (int i = k; i < bf.GetLength(); i++)
		{
			EXPECT_EQ(0, bf.GetBit(i));
		}
		return 0;
	}
	//~ParameterizedBitFields() {}
};

TEST_P(ParameterizedBitFieldsSimple, intersection_is_correct)
{
	//ParameterizedBitFieldsSimple()
	TBitField res(GetParam());
	res = bf1 & bf2;
	EXPECT_EQ(1, res.GetBit(0));
	EXPECT_EQ(0, res.GetBit(1));
	ZeroTest(res, 1);
	//~ParameterizedBitFields()
}

INSTANTIATE_TEST_CASE_P(Instantiation3,
	ParameterizedBitFieldsSimple,
	::testing::Values(20, 100));
//------------------------------------------------------------------------------------------------------------

TEST(TBitField, can_create_bitfield_with_positive_length)
{
  ASSERT_NO_THROW(TBitField bf(3));
}

TEST(TBitField, can_get_length)
{
  TBitField bf(3);

  EXPECT_EQ(3, bf.GetLength());
}

TEST(TBitField, new_bitfield_is_set_to_zero)
{
  TBitField bf(100);

  int sum = 0;
  for (int i = 0; i < bf.GetLength(); i++)
  {
    sum += bf.GetBit(i);
  }

  EXPECT_EQ(0, sum);
}

TEST(TBitField, can_set_bit)
{
  TBitField bf(10);

  EXPECT_EQ(0, bf.GetBit(3));

  bf.SetBit(3);
  EXPECT_NE(0, bf.GetBit(3));
}

TEST(TBitField, can_clear_bit)
{
  TBitField bf(10);

  int bitIdx = 3;

  bf.SetBit(bitIdx);
  EXPECT_NE(0, bf.GetBit(bitIdx));

  bf.ClrBit(bitIdx);
  EXPECT_EQ(0, bf.GetBit(bitIdx));
}

TEST(TBitField, throws_when_create_bitfield_with_negative_length)
{
  ASSERT_ANY_THROW(TBitField bf(-3));
}

TEST(TBitField, throws_when_set_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.SetBit(-3));
}

TEST(TBitField, throws_when_set_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.SetBit(11));
}

TEST(TBitField, throws_when_get_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.GetBit(-3));
}

TEST(TBitField, throws_when_get_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.GetBit(11));
}

TEST(TBitField, throws_when_clear_bit_with_negative_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.ClrBit(-3));
}

TEST(TBitField, throws_when_clear_bit_with_too_large_index)
{
  TBitField bf(10);

  ASSERT_ANY_THROW(bf.ClrBit(11));
}

TEST(TBitField, can_assign_bitfields_of_equal_size)
{
  const int size = 2;
  TBitField bf1(size), bf2(size);
  for (int i = 0; i < size; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_NE(0, bf2.GetBit(0));
  EXPECT_NE(0, bf2.GetBit(1));
}

TEST(TBitField, assign_operator_changes_bitfield_size)
{
  const int size1 = 2, size2 = 5;
  TBitField bf1(size1), bf2(size2);
  for (int i = 0; i < size1; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_EQ(2, bf2.GetLength());
}

TEST(TBitField, can_assign_bitfields_of_non_equal_size)
{
  const int size1 = 2, size2 = 5;
  TBitField bf1(size1), bf2(size2);
  for (int i = 0; i < size1; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_NE(0, bf2.GetBit(0));
  EXPECT_NE(0, bf2.GetBit(1));
}

TEST(TBitField, compare_equal_bitfields_of_equal_size)
{
  const int size = 2;
  TBitField bf1(size), bf2(size);
  for (int i = 0; i < size; i++)
  {
    bf1.SetBit(i);
  }
  bf2 = bf1;

  EXPECT_EQ(bf1, bf2);
}

TEST(TBitField, or_operator_applied_to_bitfields_of_equal_size)
{
  const int size = 4;
  TBitField bf1(size), bf2(size), expBf(size);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 0101
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 0111
  expBf.SetBit(1);
  expBf.SetBit(2);
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 | bf2);
}

TEST(TBitField, or_operator_applied_to_bitfields_of_non_equal_size)
{
  const int size1 = 4, size2 = 5;
  TBitField bf1(size1), bf2(size2), expBf(size2);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 01010
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 01110
  expBf.SetBit(1);
  expBf.SetBit(2);
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 | bf2);
}

TEST(TBitField, and_operator_applied_to_bitfields_of_equal_size)
{
  const int size = 4;
  TBitField bf1(size), bf2(size), expBf(size);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 0101
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 0001
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 & bf2);
}

TEST(TBitField, and_operator_applied_to_bitfields_of_non_equal_size)
{
  const int size1 = 4, size2 = 5;
  TBitField bf1(size1), bf2(size2), expBf(size2);
  // bf1 = 0011
  bf1.SetBit(2);
  bf1.SetBit(3);
  // bf2 = 01010
  bf2.SetBit(1);
  bf2.SetBit(3);

  // expBf = 00010
  expBf.SetBit(3);

  EXPECT_EQ(expBf, bf1 & bf2);
}

TEST(TBitField, can_invert_bitfield)
{
  const int size = 2;
  TBitField bf(size), negBf(size), expNegBf(size);
  // bf = 01
  bf.SetBit(1);
  negBf = ~bf;

  // expNegBf = 10
  expNegBf.SetBit(0);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, can_invert_large_bitfield)
{
  const int size = 38;
  TBitField bf(size), negBf(size), expNegBf(size);
  bf.SetBit(35);
  negBf = ~bf;

  for(int i = 0; i < size; i++)
    expNegBf.SetBit(i);
  expNegBf.ClrBit(35);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, invert_plus_and_operator_on_different_size_bitfield)
{
  const int firstSze = 4, secondSize = 8;
  TBitField firstBf(firstSze), negFirstBf(firstSze), secondBf(secondSize), testBf(secondSize);
  // firstBf = 0001
  firstBf.SetBit(0);
  negFirstBf = ~firstBf;
  cout << negFirstBf << endl;
  // negFirstBf = 1110

  // secondBf = 00011000
  secondBf.SetBit(3);
  secondBf.SetBit(4);

  TBitField res = (secondBf & negFirstBf);

  // testBf = 00001000
  testBf.SetBit(3);

  EXPECT_EQ(testBf, secondBf & negFirstBf);
}

TEST(TBitField, can_invert_many_random_bits_bitfield)
{
  const int size = 38;
  TBitField bf(size), negBf(size), expNegBf(size);

  std::vector<int> bits;
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(14);
  bits.push_back(16);
  bits.push_back(33);
  bits.push_back(37);

  for (unsigned int i = 0; i < bits.size(); i++)
    bf.SetBit(bits[i]);

  negBf = ~bf;

  for(int i = 0; i < size; i++)
    expNegBf.SetBit(i);
  for (unsigned int i = 0; i < bits.size(); i++)
    expNegBf.ClrBit(bits[i]);

  EXPECT_EQ(expNegBf, negBf);
}

TEST(TBitField, bitfields_with_different_bits_are_not_equal)
{
  const int size = 4;
  TBitField bf1(size), bf2(size);

  bf1.SetBit(1);
  bf1.SetBit(3);

  bf2.SetBit(1);
  bf2.SetBit(2);

  EXPECT_NE(bf1, bf2);
}
