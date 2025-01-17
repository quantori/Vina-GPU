/*

   Copyright (c) 2006-2010, The Scripps Research Institute

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Author: Dr. Oleg Trott <ot14@columbia.edu>, 
           The Olson Lab, 
           The Scripps Research Institute

*/

#ifndef VINA_MONTE_CARLO_H
#define VINA_MONTE_CARLO_H

#include "ssd.h"
#include "incrementable.h"
#include "commonMacros.h"
/*
* �ṹ��monte_carlo
* ��Ա��1.unsigned����num_steps
*		2.double����temperature
*		3.vecʸ��hunt_cap
*		4.double����min_rmsd
*		5.unsigned����num_saved_mins
*		6.double����mutation_amplitude
*		7.ssd�����ssd_par
*		8.���캯������ʼ��num_steps=2500��temperature=1.2��hunt_cap=[10, 1.5, 10], min_rmsd=0.5, num_saved_mins=50, mutation_amplitude=2
*       9.����������ò�����������Ϊ��������
*					input:model������,precalculate�ೣ������,igrid�ೣ������,precalculate�ೣ������,igrid�ೣ������,vecʸ����������,vecʸ����������,incrementable��ָ��,generator������
*					output:output_type��
*		9.����many_runs������Ϊ��������
* 					input:model������,precalculate�ೣ������,igrid�ೣ������,vecʸ����������,vecʸ����������,unsigned int������,generator������
* 					output:output_type��
*		10.����single_run������Ϊ��������
* 					input:model������,output_type�࣬precalculate�ೣ������,igrid�ೣ������,generator������
* 					output:��
*		11.��������������ò�����������Ϊ��������
* 					input:model������,output_container�����ã�precalculate�ೣ������,igrid�ೣ������,precalculate�ೣ������,igrid�ೣ������,vecʸ����������,vecʸ����������,incrementable��ָ��,generator������
* 					output:��
*					���ԭ�汾�������output_container�����ã��޷���ֵ
* 		9.����many_runs������Ϊ��������
* 					input:model������,output_container�����ã�precalculate�ೣ������,igrid�ೣ������,vecʸ����������,vecʸ����������,unsigned int������,generator������
* 					output:��
*					���ԭ�汾�������output_container�����ã��޷���ֵ
*/
struct monte_carlo {
	unsigned num_steps;
	fl temperature;
	vec hunt_cap;
	fl min_rmsd;
	sz num_saved_mins;
	fl mutation_amplitude;
	ssd ssd_par;

	int thread; //parallelism 20210917 Glinttsd
	int search_depth; // 20210813 Glinttsd
	int thread_per_call;
	int device;



	monte_carlo() : num_steps(2500), temperature(1.2), hunt_cap(10, 1.5, 10), min_rmsd(0.5), num_saved_mins(50), mutation_amplitude(2) {} // T = 600K, R = 2cal/(K*mol) -> temperature = RT = 1.2;  num_steps = 50*lig_atoms = 2500

	output_type operator()(model& m, const precalculate& p, const igrid& ig, const precalculate& p_widened, const igrid& ig_widened, const vec& corner1, const vec& corner2, incrementable* increment_me, rng& generator) const;
	output_type many_runs(model& m, const precalculate& p, const igrid& ig, const vec& corner1, const vec& corner2, sz num_runs, rng& generator) const;

	void single_run(model& m, output_type& out, const precalculate& p, const igrid& ig, rng& generator) const;
	// out is sorted
	void operator()(model& m, output_container& out, const precalculate& p, const igrid& ig, const precalculate& p_widened, const igrid& ig_widened, const vec& corner1, const vec& corner2, incrementable* increment_me, rng& generator) const;
	void many_runs(model& m, output_container& out, const precalculate& p, const igrid& ig, const vec& corner1, const vec& corner2, sz num_runs, rng& generator) const;
	std::vector<output_type> cl_to_vina(output_type_cl result_ptr[], int exhaus) const;
	void generate_uniform_position(const vec corner1, const vec corner2, std::vector<vec>& uniform_data, int exhaustiveness) const;
	//void print_process(boost::progress_display* p_d);
};

#endif

