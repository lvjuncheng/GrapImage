/***********************************************
 * ʹ�õ���Ļ����ṹʵ������������
 * \author �Ž�
 ***********************************************/

#ifndef SN3D_ALGORITHM_COMMON_BASE_TRIMESH_H
#define SN3D_ALGORITHM_COMMON_BASE_TRIMESH_H

#include "Cloud.h"
#include "Image.h"

namespace Sn3DAlgorithm
{
	///���������������
	enum MeshOpType
	{
		MeshOpType_UNION, ///< result = A U B
		MeshOpType_DIFF, ///< result = A - B
		MeshOpType_INTER, ///< result = A ^ B
		MeshOpType_XOR, ///< result = A XOR B
		MeshOpType_RESOLVE_INTER, ///< result = resolve intersection and subdiv A,B
	};

	///�������������Ƭ�࣬���������������Id
	struct SN3D_COMMON_API BaseTriFace
	{
		BaseTriFace() { vId[0] = vId[1] = vId[2] = 0; }
		BaseTriFace(int v1, int v2, int v3) {
			vId[0] = v1; vId[1] = v2; vId[2] = v3;
		}

		int vId[3];
	};

	union SN3D_COMMON_API FaceFlag
	{
		struct
		{
			bool Deleted:	1;
			bool Selected:	1;
			bool Visisted:	1;
			bool Modified:	1;
			bool Boundary:	1;
		};
		char flags;
		FaceFlag() :flags(0) {}
	} ;


	/** \brief ʹ�õ���Ļ����ṹʵ������������
	 *  \note ֻ����ʹ��һ������ͼ
	 */
	class SN3D_COMMON_API BaseTriMesh : public Cloud
	{
	public:
		///����UV����
		struct UVCoord
		{
			UVCoord(): u(0), v(0) {}
			UVCoord(Scalar uu, Scalar vv) :u(uu), v(vv) {}
			Scalar u;   ///< ������  y height [0,1]
			Scalar v;   ///< ������  x width  [0,1]
		};
		///////////////////////////////////////////////���캯������������
		BaseTriMesh();
		BaseTriMesh(const std::vector<Eigen::Vector3>& vCoords, const std::vector<BaseTriFace>& faces);
		virtual ~BaseTriMesh();

		///////////////////////////////////////////////acess and modify
		inline size_t get_fn() const;
		inline const std::vector<BaseTriFace>& get_faces() const;
		inline void get_faces(std::vector<BaseTriFace>& faces) const;
		inline const BaseTriFace& get_face(size_t id) const;
		inline BaseTriFace& get_face(size_t id);
		inline void set_face(size_t id, const BaseTriFace& face);

		//////////////////////////////////////////////add and delete
		inline virtual void reserve_fn(size_t num);
		inline virtual void resize_fn(size_t num);

		///���øú�������Ƭ�ı�ѡ״̬��ȫ���ر�
		inline virtual void set_faces(const std::vector<BaseTriFace>& faces);
		inline virtual void set_faces(std::vector<BaseTriFace>&& faces);
		inline virtual size_t add_faces(const BaseTriFace& face);

		////////////////////////////////////////////////////��ѡ���
		inline void enable_fNormal();
		inline void disable_fNormal();
		inline bool is_enable_fNormal() const;
		/** \brief�����淨��
		*/
		inline RetVal set_fNrvs(std::vector<Eigen::Vector3>&& fNrvs);
		inline const std::vector<Eigen::Vector3>& get_fNrvs() const;
		inline void get_fNrvs(std::vector<Eigen::Vector3>& fNrvs) const;
		inline const Eigen::Vector3& get_fNrv(size_t id) const;
		inline Eigen::Vector3& get_fNrv(size_t id);
		inline void set_fNrv(size_t id, const Eigen::Vector3& fNrv);

		inline void enable_fFlag();
		inline void disable_fFlag();
		inline bool is_enable_fFlag() const;
		inline const std::vector<FaceFlag>& get_fFlags() const;
		inline void get_fFlags(std::vector<FaceFlag>& fFlags) const;
		inline const FaceFlag& get_fFlag(size_t id) const;
		inline FaceFlag& get_fFlag(size_t id);
		inline void set_fFlag(size_t id, const FaceFlag& fFlag);

		///enable_fFlag��fDeleted����Ч�����disable_fFlag,�򷵻�false
		inline bool get_fDeleted(size_t id) const;
		///enable_fFlag��fDeleted����Ч
		inline void set_fDeleted(size_t id, bool flag);

		/////////////////////////////////////////////////////��ѡ��� ����
		inline void enable_texture();
		inline void disable_texture();
		inline bool is_enable_texture()const;
		inline const std::vector<Eigen::Vector3i>& get_face_texture_ids() const;
		inline void get_face_texture_ids(std::vector<Eigen::Vector3i>& textureIds) const;
		inline const Eigen::Vector3i& get_face_texture_id(size_t id) const;
		inline Eigen::Vector3i& get_face_texture_id(size_t id);
		inline void set_face_texture_id(size_t id, const Eigen::Vector3i& textureId);

		const std::vector<UVCoord>& get_texture_coords() const { return _textureCoords; }
		std::vector<UVCoord>& get_texture_coords() { return _textureCoords; }
		void set_texture_coords(const std::vector<UVCoord>& coord) { _textureCoords = coord; }

		const ImageRGBb& get_texture_image() const { return _textureImage; }
		ImageRGBb& get_texture_image() { return _textureImage; }
		void set_texture_image(const ImageRGBb& image) { _textureImage = image; }

		/////////////////////////////////////////////////////////////////Ӳ�����ڴ����ݽ���
		///�����ݱ�����Ӳ�̡�UNDO:û�п�������
		virtual int swap_out_data();
		///�����ݴ�Ӳ�̶�������UNDO:û�п�������
		virtual int swap_in_data();

		///////////////////////////////////////////////////////////////////�ڴ����
		///����ڴ�
		virtual void clear();
		///��ɾ�������ݿռ��ͷų�����UNDO:û�п�������
		virtual void squeeze();

		///////////////////////////////////////////////////////////////////operator
		///ģ�;���һ������任
		virtual void rigid_transformation(const RigidMatrix& mat);

        ///���µ�ķ�����
        void update_VNrv();

		/** \brief���õ㷨��
		*/
		inline RetVal set_VNrvs(std::vector<Eigen::Vector3>&& vNrvs);


        ///�õ���ķ�����
        void compute_VNrv(std::vector<Eigen::Vector3>& vNrvs) const;

        ///������ķ�����
        void update_FNrv();

        ///�õ���ķ�����
        void compute_FNrv(std::vector<Eigen::Vector3>& FNrvs) const;

		/*!
		 * @brief   	��ת����	- ������ ��Ƭ�ڶ���˳����Ƭ���򡢶��㷨��
		 * @author  	yangxl [yangxuelian@shining3d.com]
		 * @date    	2015-05
		 * UNDO:û�п�������
		 */
		void reverse_norm();

		///��֤�����Ƿ�Ϸ�.UNDO:û�п�������
		virtual bool is_legal() const;
		///�޲����Ϸ�����.UNDO:û�п�������
		virtual void repair_illegal();

	protected:
		std::vector<BaseTriFace> _faces;  ///< �����������Ƭ��Ϣ

		//��ѡ���
		bool _hasFNrv;                       
		std::vector<Eigen::Vector3> _fNrvs;   ///< ��Ƭ�ķ�����
		bool _hasFFlag;
		std::vector<FaceFlag> _fFlags;    ///< ������Ƭ��־λ
		bool _hasTexture;

		/** \brief ��������(tu, tv)ͨ�����¹�ʽת��ΪͼƬ����������(iu, iv)
		 *   height width ΪͼƬ�����س���
		 *   iu = height - 1 - tv*height
		 *   iv = tu*width;
		 */
		std::vector<UVCoord> _textureCoords;  ///<��������
		std::vector<Eigen::Vector3i> _faceTextureIds; ///<��Ƭ��ÿ���������Id
		ImageRGBb _textureImage;                   ///<����ͼ 
	};

	/**	\brief	����ϲ�(ֻ����������)
	*	\param[in, out]			dst								���ϲ�����1
	*	\param[in]				src								���ϲ�����2
	*/
	RetVal SN3D_COMMON_API merge_mesh(
		BaseTriMesh& dst,
		const BaseTriMesh& src
		);

	/**	\brief	����ϲ�(ֻ����������)
	*	\param[in]				meshsToMerge						���ϲ�����
	*	\param[out]				mesh								�ϲ����
	*/
	RetVal SN3D_COMMON_API merge_mesh(
		const std::vector<BaseTriMesh>& meshsToMerge,
		BaseTriMesh& mesh
		);

	/**	\brief	����ϲ�(ֻ����������)
	*	\param[in]				meshsToMerge						���ϲ�����
	*	\param[out]				mesh								�ϲ����
	*/
	RetVal SN3D_COMMON_API merge_mesh(
		const std::vector<const BaseTriMesh*>& meshsToMerge,
		BaseTriMesh& mesh
		);

	///////////////////////////////////////////////acess and modify
	inline size_t BaseTriMesh::get_fn() const
	{
		return _faces.size();
	}
	inline const std::vector<BaseTriFace>& BaseTriMesh::get_faces() const
	{
		return _faces;
	}
	inline void BaseTriMesh::get_faces(std::vector<BaseTriFace>& faces) const
	{
		faces = _faces;
	}
	inline const BaseTriFace& BaseTriMesh::get_face(size_t id) const
	{
		Sn_Ensure(get_fn() > id);
		return _faces[id];
	}
	inline BaseTriFace& BaseTriMesh::get_face(size_t id)
	{
		Sn_Ensure(get_fn() > id);
		return _faces[id];
	}
	inline void BaseTriMesh::set_face(size_t id, const BaseTriFace& face)
	{
		Sn_Ensure(get_fn() > id);
		_faces[id] = face;
	}

	//////////////////////////////////////////////add and delete
	inline void BaseTriMesh::reserve_fn(size_t num)
	{
		_faces.reserve(num);
		if(is_enable_fNormal()) _fNrvs.reserve(num);
		if(is_enable_fFlag()) _fFlags.reserve(num);
		if(is_enable_texture()) _faceTextureIds.reserve(num);
	}
	inline void BaseTriMesh::resize_fn(size_t num)
	{
		_faces.resize(num);
		if(is_enable_fNormal()) _fNrvs.resize(num);
		if(is_enable_fFlag()) _fFlags.resize(num);
		if(is_enable_texture()) _faceTextureIds.resize(num);
	}

	///���øú�������Ƭ�ı�ѡ״̬��ȫ���ر�
	inline void BaseTriMesh::set_faces(const std::vector<BaseTriFace>& faces)
	{
		_faces = faces;
		disable_fNormal();
		disable_fFlag();
		disable_texture();
	}
	inline void BaseTriMesh::set_faces(std::vector<BaseTriFace>&& faces)
	{
		_faces = std::move(faces);
		disable_fNormal();
		disable_fFlag();
		disable_texture();
	}
	inline size_t BaseTriMesh::add_faces(const BaseTriFace& face)
	{
		_faces.push_back(face);
		if(is_enable_fNormal()) _fNrvs.push_back(Eigen::Vector3(0, 0, -1));
		if(is_enable_fFlag()) _fFlags.push_back(FaceFlag());
		if(is_enable_texture()) _faceTextureIds.push_back(Eigen::Vector3i(0, 0, 0));
		return _faces.size()-1;
	}

	////////////////////////////////////////////////////��ѡ���
	inline void BaseTriMesh::enable_fNormal()
	{
		if(!is_enable_fNormal())
		{
			_hasFNrv = true;
			_fNrvs.resize(get_fn());
		}
	}
	inline void BaseTriMesh::disable_fNormal()
	{
		if(is_enable_fNormal())
		{
			_hasFNrv = false;
			std::vector<Eigen::Vector3>().swap(_fNrvs);
		}
	}
	inline bool BaseTriMesh::is_enable_fNormal() const
	{
		return _hasFNrv;
	}
	inline RetVal BaseTriMesh::set_fNrvs(std::vector<Eigen::Vector3>&& fNrvs)
	{
		if (fNrvs.size() != _faces.size())
		{
			return RetVal_ILLEGAL_INPUT;
		}
		_hasFNrv = true;
		_fNrvs = fNrvs;
		return RetVal_OK;
	}
	inline RetVal BaseTriMesh::set_VNrvs(std::vector<Eigen::Vector3>&& vNrvs)
	{
		if (vNrvs.size() != _vCoords.size())
		{
			return RetVal_ILLEGAL_INPUT;
		}
		_hasVNrv = true;
		_vNrvs = vNrvs;
		return RetVal_OK;
	}
	inline const std::vector<Eigen::Vector3>& BaseTriMesh::get_fNrvs() const
	{
		return _fNrvs;
	}
	inline void BaseTriMesh::get_fNrvs(std::vector<Eigen::Vector3>& fNrvs) const
	{
		fNrvs = _fNrvs;
	}
	inline const Eigen::Vector3& BaseTriMesh::get_fNrv(size_t id) const
	{
		Sn_Ensure(_fNrvs.size() > id);
		return _fNrvs[id];
	}
	inline Eigen::Vector3& BaseTriMesh::get_fNrv(size_t id)
	{
		Sn_Ensure(_fNrvs.size() > id);
		return _fNrvs[id];
	}
	inline void BaseTriMesh::set_fNrv(size_t id, const Eigen::Vector3& fNrv)
	{
		Sn_Ensure(_fNrvs.size() > id);
		_fNrvs[id] = fNrv;
	}

	inline void BaseTriMesh::enable_fFlag()
	{
		if(!is_enable_fFlag())
		{
			_hasFFlag = true;
			_fFlags.resize(get_fn());
		}
	}
	inline void BaseTriMesh::disable_fFlag()
	{
		if(is_enable_fFlag())
		{
			_hasFFlag = false;
			std::vector<FaceFlag>().swap(_fFlags);
		}
	}
	inline bool BaseTriMesh::is_enable_fFlag() const
	{
		return _hasFFlag;
	}
	inline const std::vector<FaceFlag>& BaseTriMesh::get_fFlags() const
	{
		return _fFlags;
	}
	inline void BaseTriMesh::get_fFlags(std::vector<FaceFlag>& fFlags) const
	{
		fFlags = _fFlags;
	}
	inline const FaceFlag& BaseTriMesh::get_fFlag(size_t id) const
	{
		Sn_Ensure(_fFlags.size() > id);
		return _fFlags[id];
	}
	inline FaceFlag& BaseTriMesh::get_fFlag(size_t id)
	{
		Sn_Ensure(_fFlags.size() > id);
		return _fFlags[id];
	}
	inline void BaseTriMesh::set_fFlag(size_t id, const FaceFlag& fFlag)
	{
		Sn_Ensure(_fFlags.size() > id);
		_fFlags[id] = fFlag;
	}

	inline bool BaseTriMesh::get_fDeleted(size_t id) const
	{
		if(!_hasFFlag) return false;
		Sn_Ensure(_fFlags.size() > id);
		return _fFlags[id].Deleted;
	}
	inline void BaseTriMesh::set_fDeleted(size_t id, bool flag)
	{
		Sn_Ensure(_fFlags.size() > id);
		_fFlags[id].Deleted = flag;
	}

	/////////////////////////////////////////////////////��ѡ��� ����
	inline void BaseTriMesh::enable_texture()
	{
		if(!is_enable_texture())
		{
			_hasTexture = true;
			_faceTextureIds.resize(get_fn());
		}
	}

	inline void BaseTriMesh::disable_texture()
	{
		_hasTexture = false;
		std::vector<Eigen::Vector3i>().swap(_faceTextureIds);
		std::vector<UVCoord>().swap(_textureCoords);
		_textureImage.set_size(0, 0);
	}

	inline bool BaseTriMesh::is_enable_texture()const
	{
		return _hasTexture;
	}

	inline const std::vector<Eigen::Vector3i>& BaseTriMesh::get_face_texture_ids() const
	{
		return _faceTextureIds;
	}

	inline void BaseTriMesh::get_face_texture_ids(std::vector<Eigen::Vector3i>& textureIds) const
	{
		textureIds = _faceTextureIds;
	}

	inline const Eigen::Vector3i& BaseTriMesh::get_face_texture_id(size_t id) const
	{
		Sn_Ensure(id < get_fn());
		return _faceTextureIds[id];
	}

	inline Eigen::Vector3i& BaseTriMesh::get_face_texture_id(size_t id)
	{
		Sn_Ensure(id < get_fn());
		return _faceTextureIds[id];
	}

	inline void BaseTriMesh::set_face_texture_id(size_t id, const Eigen::Vector3i& textureId)
	{
		Sn_Ensure(id < get_fn());
		_faceTextureIds[id] = textureId;
	}

}//namepsace

#endif //SN3D_ALGORITHM_COMMON_BASE_TRIMESH_H