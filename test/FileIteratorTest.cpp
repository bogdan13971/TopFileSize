//#include <gtest/gtest.h>
//#include <FileIterator.hpp>
//
//class FileTestIterator : public testing::Test
//{
//protected:
//	std::atomic_int actual_dirs = 0;
//	std::atomic_int actual_files = 0;
//
//	FileIterator fileIterator;
//
//	FileTestIterator()
//		: fileIterator("D:\\projects\\boost_1_82_0",
//			[&](const auto& path)
//			{
//				if (boost::filesystem::is_directory(path))
//				{
//					actual_dirs++;
//				}
//				else
//				{
//					actual_files++;
//				}
//			})
//	{}
//
//	static const unsigned int total_files = 78'130;
//	static const unsigned int total_dirs = 7'931;
//	const size_t num_threads = 4;
//};
//
//TEST_F(FileTestIterator, SyncIteration)
//{
//	fileIterator.sync_iteration();
//
//	EXPECT_EQ(actual_files, total_files);
//	EXPECT_EQ(actual_dirs, total_dirs);
//}
//
//TEST_F(FileTestIterator, AsyncIteration)
//{
//	fileIterator.async_iteration(num_threads);
//
//	EXPECT_EQ(actual_files, total_files);
//	EXPECT_EQ(actual_dirs, total_dirs);
//}
//
//TEST_F(FileTestIterator, AsyncIterationV2)
//{
//	fileIterator.async_iteration_v2(num_threads);
//
//	EXPECT_EQ(actual_files, total_files);
//	EXPECT_EQ(actual_dirs, total_dirs);
//}