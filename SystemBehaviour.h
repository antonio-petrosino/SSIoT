#include "ClassDefinition.h"

Service *ServicesCreation(int n_services){

    const float possible_power_cost[] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
    const int possible_cpu_req[]      = {6  ,6  ,  6, 10, 10, 10, 14, 14};
    
    Service* arrayOfServices = new Service[n_services];    
    
    int length_possible = sizeof(possible_power_cost)/ sizeof(float);    

	int choosenIndex, randomCpuReq, temp_power_cost, temp_cpu_req;
	float randomNumber;
	
		
	for(int i=0; i<n_services; i++){	
		choosenIndex = rand() % length_possible;
		
		randomNumber = possible_power_cost[choosenIndex];
		randomCpuReq = possible_cpu_req[choosenIndex];
		
		arrayOfServices[i].SetService(i+1,randomNumber,randomCpuReq);
	}	
	
	cout << endl;
	cout << "List of generated services:" <<endl;
	for(int i=0; i<n_services; i++){
		arrayOfServices[i].PrintService();
	}
	
	cout << "#########################################" <<endl;
	
	return arrayOfServices;
}


Device* DeviceCreation(int n_devices, int n_services, int n_master, Service *defined_services, Master *defined_master){
	
	Device* arrayOfDevice = new Device[n_devices];
	
	const float possible_total_power[] = {0.2,0.4,0.6,0.8};
	const int possible_class[] = {1,2,2,3};	
	const int possible_clock_speed[]={400, 1000, 1000, 2000};
	
	int length_pos_total_p = sizeof(possible_total_power)/ sizeof(float);
	
	const int possible_owner[] = {1,2,3,4,5,6,7,8,9,10};
	int length_pos_owner = sizeof(possible_owner)/ sizeof(int);
	
	const int length_pos_loc = n_master;
	vector<int> possible_location(length_pos_loc);
	
	for(int i=0;i<length_pos_loc; i++){
		possible_location[i] = i+1;
	}
				
	int choosenIndex, id_owner, id_man, loc, d_class, clock_s;
	float total_power;
	
	for(int i=0; i<n_devices; i++){

		choosenIndex = rand() % length_pos_total_p;
		total_power = possible_total_power[choosenIndex];
		
		if (total_power > 0.5) 
		{
			int possible_high_class[] = {2,2,2,3,3,3,3,3,3,3};	
			int length_high_class = sizeof(possible_high_class)/ sizeof(int);
			choosenIndex = rand() % length_high_class;
			d_class = possible_high_class[choosenIndex];
						
			if (d_class == 3){
				clock_s = possible_clock_speed[3];
			}else{
				clock_s = possible_clock_speed[2];
			}
			
		}else{
			d_class = possible_class[choosenIndex];
			clock_s = possible_clock_speed[choosenIndex];
		}
		
		choosenIndex = rand() % length_pos_owner;
		id_owner = possible_owner[choosenIndex];
		
		choosenIndex = rand() % length_pos_owner;
		id_man = possible_owner[choosenIndex];
		
		choosenIndex = rand() % length_pos_loc;
		loc = possible_location[choosenIndex];
		
		arrayOfDevice[i].GenerateDevice(i+1,total_power, id_owner, id_man, loc, d_class, clock_s);
	
		// SetServicesList
		const int possible_num_service_class[] = {1,1,1,1,1,1,1,1,1,2};	// 90% - 1 -- 10% - 2
		int length_possible_num_service_class = sizeof(possible_num_service_class)/ sizeof(int);	
		choosenIndex = rand() % length_possible_num_service_class;
					
		vector<int> suitable_services; // elenco di id
		
		for(int j=0;j<n_services;j++){
			if(arrayOfDevice[i].total_power >= (defined_services[j].GetPowerCost() *2))
			{ 
				suitable_services.push_back(defined_services[j].GetServiceId());											
			}
		}
					
		vector<int> choosen_services;
		int choosenSuitableService;
				
		for(int j=0;j<possible_num_service_class[choosenIndex];j++){			
		
		    if(suitable_services.size() > 0){			
		    choosenSuitableService = rand() % suitable_services.size();
			choosen_services.push_back(suitable_services[choosenSuitableService]);			
			//suitable_services.esase(suitable_services.begin(), suitable_services.end(), suitable_services[choosenSuitableService], suitable_services.end());
			suitable_services.erase(suitable_services.begin() + choosenSuitableService);
		    }
		}
				
		arrayOfDevice[i].SetServicesList(choosen_services); // assegno finalmente i servizi definiti

		
		for(int j=0;j<choosen_services.size();j++){		
				for(int k=0;k<n_master;k++){
					vector<int> service_to_find = defined_master[k].GetAllServices();					
					for(int s=0;s<service_to_find.size();s++){						
						if(service_to_find[s] == choosen_services[j]){	
												
//							Registered_Device new_device_to_add; 
//							new_device_to_add.device_id = arrayOfDevice[i].device_id;					
//							vector<Friend_Record> empty_friend_list;
//							new_device_to_add.friend_info = empty_friend_list;
							
							defined_master[k].AddDevice(arrayOfDevice[i].device_id);
							arrayOfDevice[i].master_node_id_list.push_back(defined_master[k].GetID());
						}
					}					
				}	
		}
		// SetMasterNodeList

	}
	
	// SetFriendRecord // va fatta alla fine del processo di creazione
	
	for(int i=0; i<n_devices; i++){
		arrayOfDevice[i].PrintDevice();
	}

	return arrayOfDevice;
}


void GenerateSocialRel(int n_devices, Device *defined_devices){
	
	Friend_Record new_social_rel{};
	int check_social = 0;
			
			
	for(int i=0;i<n_devices;i++){
		for(int j=0;j<n_devices;j++){
			check_social = 0;
			
			if(defined_devices[i].GetID() != defined_devices[j].GetID()){
				if(defined_devices[i].id_manufacturer == defined_devices[j].id_manufacturer){					
					new_social_rel.sociality_factor = 0.9;
					new_social_rel.type_rel = "POR";
					check_social = 1;
				}
				else if(defined_devices[i].id_owner == defined_devices[j].id_owner){
					new_social_rel.sociality_factor = 0.8;
					new_social_rel.type_rel = "OOR";
					check_social = 1;
				}
				else if(defined_devices[i].location == defined_devices[j].location) {
					new_social_rel.sociality_factor = 0.5;
					new_social_rel.type_rel = "C-LOR";
					check_social = 1;			 	
				}
			}
			
			if(check_social>0){
				new_social_rel.friend_device_id = defined_devices[j].GetID();
				defined_devices[i].AddFriendRecord(new_social_rel);
			}
			
		}
	}
}

Master* MasterCreation(int n_master, int n_services, Service *defined_services){
	Master* arrayOfMaster = new Master[n_master];
	
	for(int i=0;i<n_master;i++){ 
		arrayOfMaster[i].SetMaster(i+1); // anche location				
	}
	
	int given_master_id = 0;	
	for(int i=0;i<n_services;i++){							
		arrayOfMaster[i % n_master].AddNewService(defined_services[i].GetServiceId());
		
	}
		
//	for(int i=0; i<n_master; i++){
//		arrayOfMaster[i].PrintMaster();
//	}

	return arrayOfMaster;
}
