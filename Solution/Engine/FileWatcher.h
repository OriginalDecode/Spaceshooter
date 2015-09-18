#pragma once

#include <functional>

namespace Prism
{
	class FileWatcher
	{
	public:
		FileWatcher();

		void WatchFile(const std::string& aFile, std::function<void()> aCallBack);

		void CheckFiles();

	private:
		struct FileData
		{
			std::string myFilePath;
			std::function<void()> myCallBack;
			FILETIME myFileTime;
		};

		CU::GrowingArray<FileData> myFileDatas;
	};
}