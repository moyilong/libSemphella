#pragma once

#ifndef __GCC_OVERRIDE_DEBUG_STAT_CHANGE	//GCC���Ǵ������״̬
#define __ALLOW_DEBUG_STAT_CHANGE		//��������޸ĵ���״̬
#endif

#ifdef __ALLOW_DEBUG_STAT_CHANGE
#define __ALLOW_DYMANIC_DEBUG			//����̬�޸ĵ���״̬
#endif

//����ʹ��NV-CUDA����
#ifndef __LINUX__
#ifndef __WNO_CUDA
#define ALLOW_NVIDIA_CUDA_COMPUTING
#endif
#endif

#ifndef __DEFAULT_DEBUG_STAT
#define __DEFAULT_DEBUG_STAT	false	//Ĭ�ϵ���״̬
#endif

//#define __WNO_OMP						//����OpenMPͷ

#define __ALLOW_LOWMEM_PREFER			//ʹ�õ��ڴ��Ż�����

#ifndef MTYPE
#define MTYPE	float					//��ѧ�⸡������
#endif

/*����*/
#define DEFAULT_DEBUG_STAT  __DEFAULT_DEBUG_STAT
