
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
	* @brief   		�浵�������ص�����
	* @author  		yangxl [yangxuelian@shining3d.com]
	* @date    		12/26/2016
	*/
	class SN3D_COMMON_API DataArchiveConfig {
	public:
		DataArchiveConfig() : _storePath("./"), _enabled(false), _writeMode(true), _timeStamp(false), _threadIdEnabled(false), _encodeFile(false) {}
		~DataArchiveConfig() {};

		/*!
		* @brief   		���ô洢·��
		* @param[in]	path_name		·������
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/26/2016
		*/
		void set_store_path(const std::string& path_name);


		/*!
		* @brief   		��ȡ�洢·��
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/26/2016
		*/
		std::string get_store_path() const
		{
			return _storePath;
		}

		/*!
		* @brief   		���ÿ���
		* @param[in]	open			�Ƿ���
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/28/2016
		*/
		void set_enable(bool enabled)
		{
			_enabled = enabled;
		}

		/*!
		* @brief   		��ѯ�Ƿ���
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/28/2016
		*/
		bool is_enabled() const
		{
			return _enabled;
		}

		/*!
		* @brief   		���ö�дģʽ
		* @param[in]	write_mode		�Ƿ�Ϊдģʽ
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/27/2016
		*/
		void set_write_mode(bool write_mode)
		{
			_writeMode = write_mode;
		}

		/*!
		* @brief   		��ȡ��дģʽ
		* @author  		yangxl [yangxuelian@shining3d.com]
		* @date    		12/27/2016
		*/
		bool is_write_mode() const
		{
			return _writeMode;
		}

		/*!
		 * @fn	inline bool DataArchiveConfig::has_time_stamp() const
		 * @brief	�Ƿ�����ʱ���
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		bool is_time_stamp_enabled() const { return _timeStamp; }

		/*!
		 * @fn	inline void DataArchiveConfig::set_time_stamp(bool timeStamp)
		 * @brief	���ÿ���ʱ���
		 * @param	timeStamp	�Ƿ���
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		void set_time_stamp(bool timeStamp) { _timeStamp = timeStamp; }

		/*!
		 * @fn	static std::string DataArchiveConfig::get_current_ts_string();
		 * @brief	���ݵ�ǰʱ�����ʱ����ַ���.
		 * @return	�����ַ���.
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2017/11/23.
		 */
		static std::string get_current_ts_string();

		/*!
		* @fn	inline bool DataArchiveConfig::is_thread_enabled() const
		* @brief	�Ƿ������̺߳�
		* @author	qiukaijia[qiukaijia@shining3d.com].
		* @date	2018/03/22.
		*/
		bool is_thread_enabled() const { return _threadIdEnabled; }

		/*!
		* @fn	inline bool DataArchiveConfig::set_enable_thread() const
		* @brief	���ÿ����̺߳�
		* @author	qiukaijia[qiukaijia@shining3d.com].
		* @date	2018/03/22.
		*/
		void set_enable_thread(bool threadIdEnabled){ _threadIdEnabled = threadIdEnabled; }

		/*!
		 * @fn	bool DataArchiveConfig::is_encode_file_enabled() const
		 * @brief	�Ƿ�����ļ���
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/6/27.
		 */
		bool is_encode_file_enabled() const { return _encodeFile; }

		/*!
		 * @fn	void DataArchiveConfig::set_encode_file(bool encodeFile)
		 * @brief	�����Ƿ�����ļ���
		 * @author	jiayanming[jiayanming01@shining3d.com].
		 * @date	2018/6/27.
		 */
		void set_encode_file(bool encodeFile) { _encodeFile = encodeFile; }

		void encode_file_name(const std::string& input, std::string& output);
	private:
		std::string		_storePath;		///< �ļ��洢Ŀ¼
		bool			_enabled;		///< �Ƿ������ݴ浵ģʽ
		bool			_writeMode;		///< дģʽ
		bool			_timeStamp;		///< �Ƿ�ʹ��ʱ���
		bool			_threadIdEnabled;		///< �Ƿ����̺߳�
		bool _encodeFile;//!< �Ƿ�����ļ���
	};
} // Sn3DAlgorithm

extern Sn3DAlgorithm::DataArchiveConfig SN3D_COMMON_API	gDAConfig;			// ȫ�ֱ���

#endif // SN3D_ALGORITHM_COMMON_DATA_ARCHIVE_CONFIG_H
