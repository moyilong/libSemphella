#pragma once

#ifndef __GCC_OVERRIDE_DEBUG_STAT_CHANGE	//GCC���Ǵ������״̬
#define __ALLOW_DEBUG_STAT_CHANGE		//��������޸ĵ���״̬
#endif

#ifdef __ALLOW_DEBUG_STAT_CHANGE
#define __ALLOW_DYMANIC_DEBUG			//����̬�޸ĵ���״̬
#endif

#ifndef __DEFAULT_DEBUG_STAT
#define __DEFAULT_DEBUG_STAT	true	//Ĭ�ϵ���״̬
#endif

//#define __WNO_OMP						//����OpenMPͷ

//#define __ALLOW_LOWMEM_PREFER			//ʹ�õ��ڴ��Ż�����

#ifndef MTYPE
#define MTYPE	float					//��ѧ�⸡������
#endif

#ifndef MDEBUG_STAT
#define MDEBUG_STAT true				//�ڴ����ģʽ
#endif

/*����*/
#define DEFAULT_DEBUG_STAT  __DEFAULT_DEBUG_STAT

/*DLL��������*/

#ifdef __LINUX__

#define CAPI

#else

#ifdef __DLL_EXPORT
#define CAPI	 __declspec(dllexport)
#else
#define CAPI	 __declspec(dllimport)
#endif

#endif
