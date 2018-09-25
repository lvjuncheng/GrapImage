
#ifndef SN3D_ALGORITHM_COMMON_DATA_ARCHIVE_CONFIG_H
#define SN3D_ALGORITHM_COMMON_DATA_ARCHIVE_CONFIG_H

// STL
#include <string>
#include <thread>
#include <sstream>
#include "./Traits.h"

namespace Sn3DAlgorithm {
	/*!
	* @class        DataArchiveConfig
	* @brief   		存档与加载相关的设置
	* @author  		yangxl [yangxuelian@shining3d.com]
	* @date    		12/26/2016
	*/
	class SN3D_COMMON_API DataArchiveConfig {
	public:
		DataArchiveConfig() : _storePath("./"), _enabled(false), _writeMode(true), _timeStamp(false), _threadIdEnabled(false), _encodeFile(false) {}
		~DataArchiveConfig() {};

		/*!
		* @brief   		设置存储路径
		* @param[in]	path_name		路径名称
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/26/2016
		*/
		void set_store_path(const std::string& path_name);


		/*!
		* @brief   		获取存储路径
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/26/2016
		*/
		std::string get_store_path() const
		{
			return _storePath;
		}

		/*!
		* @brief   		设置开关
		* @param[in]	open			是否开启
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/28/2016
		*/
		void set_enable(bool enabled)
		{
			_enabled = enabled;
		}

		/*!
		* @brief   		查询是否开启
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/28/2016
		*/
		bool is_enabled() const
		{
			return _enabled;
		}

		/*!
		* @brief   		设置读写模式
		* @param[in]	write_mode		是否为写模式
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/27/2016
		*/
		void set_write_mode(bool write_mode)
		{
			_writeMode = write_mode;
		}

		/*!
		* @brief   		获取读写模式
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/27/2016
		*/
		bool is_write_mode() const
		{
			return _writeMode;
		}

		/*!
		 * @fn	inline bool DataArchiveConfig::has_time_stamp() const
		 * @brief	是否开启了时间戳
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		bool is_time_stamp_enabled() const { return _timeStamp; }

		/*!
		 * @fn	inline void DataArchiveConfig::set_time_stamp(bool timeStamp)
		 * @brief	设置开启时间戳
		 * @param	timeStamp	是否开启
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		void set_time_stamp(bool timeStamp) { _timeStamp = timeStamp; }

		/*!
		 * @fn	static std::string DataArchiveConfig::get_current_ts_string();
		 * @brief	根据当前时间计算时间戳字符串.
		 * @return	生成字符串.
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		static std::string get_current_ts_string();

		/*!
		* @fn	inline bool DataArchiveConfig::is_thread_enabled() const
		* @brief	是否开启了线程号
		* @author	qiukaijia[qiukaijia@shining3d.com].
		* @date	2018/03/22.
		*/
		bool is_thread_enabled() const { return _threadIdEnabled; }

		/*!
		* @fn	inline bool DataArchiveConfig::set_enable_thread() const
		* @brief	设置开启线程号
		* @author	qiukaijia[qiukaijia@shining3d.com].
		* @date	2018/03/22.
		*/
		void set_enable_thread(bool threadIdEnabled){ _threadIdEnabled = threadIdEnabled; }

		/*!
		 * @fn	bool DataArchiveConfig::is_encode_file_enabled() const
		 * @brief	是否加密文件名
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/6/27.
		 */
		bool is_encode_file_enabled() const { return _encodeFile; }

		/*!
		 * @fn	void DataArchiveConfig::set_encode_file(bool encodeFile)
		 * @brief	设置是否加密文件名
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/6/27.
		 */
		void set_encode_file(bool encodeFile) { _encodeFile = encodeFile; }

		void encode_file_name(const std::string& input, std::string& output);
	private:
		std::string		_storePath;		///< 文件存储目录
		bool			_enabled;		///< 是否开启数据存档模式
		bool			_writeMode;		///< 写模式
		bool			_timeStamp;		///< 是否使用时间戳
		bool			_threadIdEnabled;		///< 是否开启线程号
		bool _encodeFile;//!< 是否加密文件名
	};
} // Sn3DAlgorithm

extern Sn3DAlgorithm::DataArchiveConfig SN3D_COMMON_API	gDAConfig;			// 全局变量

#endif // SN3D_ALGORITHM_COMMON_DATA_ARCHIVE_CONFIG_H
