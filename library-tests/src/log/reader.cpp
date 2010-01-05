/*
 * reader.cpp
 *
 *  Created on: Dec 31, 2009
 *      Author: dc
 */


#include <stdexcept>
#include <cstdio>

#include <gtest/gtest.h>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <barrett/units.h>
#include <barrett/log/reader.h>
#include <barrett/log/writer.h>


namespace {
using namespace barrett;


TEST(LogReaderTest, CtorThrows) {
	// TODO(dc): test this!
}

TEST(LogReaderTest, Double) {
	char tmpFile[L_tmpnam];
	ASSERT_TRUE(std::tmpnam(tmpFile) != NULL);

	double d = -23425.0000001;

	log::Writer<double> lw(tmpFile);
	lw.putRecord(d);
	lw.close();

	log::Reader<double> lr(tmpFile);
	EXPECT_EQ(1, lr.numRecords());
	EXPECT_EQ(d, lr.getRecord());
	EXPECT_THROW(lr.getRecord(), std::underflow_error);
	lr.close();

	std::remove(tmpFile);
}

TEST(LogReaderTest, TupleA) {
	typedef boost::tuple<double, double> tuple_type;

	char tmpFile[L_tmpnam];
	ASSERT_TRUE(std::tmpnam(tmpFile) != NULL);

	tuple_type d(7.2, 803.0);

	log::Writer<tuple_type> lw(tmpFile);
	lw.putRecord(d);
	lw.close();

	log::Reader<tuple_type> lr(tmpFile);
	EXPECT_EQ(1, lr.numRecords());
	EXPECT_EQ(d, lr.getRecord());
	EXPECT_THROW(lr.getRecord(), std::underflow_error);
	lr.close();

	std::remove(tmpFile);
}

TEST(LogReaderTest, TupleB) {
	typedef boost::tuple<units::Array<15>, double, double, units::JointTorques<3> > tuple_type;

	char tmpFile[L_tmpnam];
	ASSERT_TRUE(std::tmpnam(tmpFile) != NULL);

	tuple_type d;
	d.get<0>() <<	23,	54,		34,		4,		25,
					23,	6,		46,		23,		-6,
					11,	868,	12312,	-44.2,	1;
	d.get<1>() = 2323823e-12;
	d.get<2>() = -0.5;
	d.get<3>() << 2.23, 867, -34.78e6;


	log::Writer<tuple_type> lw(tmpFile);
	lw.putRecord(d);
	lw.close();

	log::Reader<tuple_type> lr(tmpFile);
	EXPECT_EQ(1, lr.numRecords());
	EXPECT_EQ(d, lr.getRecord());
	EXPECT_THROW(lr.getRecord(), std::underflow_error);
	lr.close();

	std::remove(tmpFile);
}

TEST(LogReaderTest, Array) {
	char tmpFile[L_tmpnam];
	ASSERT_TRUE(std::tmpnam(tmpFile) != NULL);

	units::Array<15> d;
	d <<	23,	54,		34,		4,		25,
			23,	6,		46,		23,		-6,
			11,	868,	12312,	-44.2,	1;

	log::Writer<units::Array<15> > lw(tmpFile);
	lw.putRecord(d);
	lw.close();

	log::Reader<units::Array<15> > lr(tmpFile);
	EXPECT_EQ(1, lr.numRecords());
	EXPECT_EQ(d, lr.getRecord());
	EXPECT_THROW(lr.getRecord(), std::underflow_error);
	lr.close();

	std::remove(tmpFile);
}

TEST(LogReaderTest, SeveralRecords) {
	char tmpFile[L_tmpnam];
	ASSERT_TRUE(std::tmpnam(tmpFile) != NULL);

	double ds[] = {3e7, -12, 432, 8.888};
	size_t n = sizeof(ds)/sizeof(double);

	log::Writer<double> lw(tmpFile);
	for (size_t i = 0; i < n; ++i) {
		lw.putRecord(ds[i]);
	}
	lw.close();

	log::Reader<double> lr(tmpFile);
	EXPECT_EQ(n, lr.numRecords());
	for (size_t i = 0; i < n; ++i) {
		EXPECT_EQ(ds[i], lr.getRecord());
	}
	EXPECT_THROW(lr.getRecord(), std::underflow_error);
	lr.close();

	std::remove(tmpFile);
}


}
