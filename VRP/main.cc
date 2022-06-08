#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#include "google/protobuf/duration.pb.h"
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

const int AJ = 1000000;

namespace operations_research {
struct DataModel {
	std::vector<std::vector<double> > distance_matrix;
	std::vector<int64_t> demands;
	std::vector<int64_t> vehicle_capacities;
	int num_vehicles;
	const RoutingIndexManager::NodeIndex depot { 0 };
};

void PrintSolution(const DataModel &data, const RoutingIndexManager &manager,
		const RoutingModel &routing, const Assignment &solution) {
	double total_distance { 0 };
	int cnt = 1;
	double total_load { 0 };
	for (int vehicle_id = 0; vehicle_id < data.num_vehicles; ++vehicle_id) {
		int64_t index = routing.Start(vehicle_id);
		//LOG(INFO) << "Route for Vehicle " << vehicle_id << ":";
		double route_distance { 0 };
		double route_load { 0 };
		std::stringstream route;
		while (routing.IsEnd(index) == false) {
			int64_t node_index = manager.IndexToNode(index).value();
			route_load += data.demands[node_index];
			route << node_index << " Load(" << route_load << ") -> ";
			int64_t previous_index = index;
			index = solution.Value(routing.NextVar(index));
			route_distance += routing.GetArcCostForVehicle(previous_index,
					index, int64_t { vehicle_id });
		}
		total_distance += route_distance;
		total_load += route_load;
	}
	LOG(INFO) << "Total distance of all routes: " << total_distance/AJ << "m";
}

void VrpCapacity(DataModel& data) {

	RoutingIndexManager manager(data.distance_matrix.size(), data.num_vehicles,
			data.depot);

	RoutingModel routing(manager);

	const int transit_callback_index = routing.RegisterTransitCallback(
			[&data, &manager](int64_t from_index, int64_t to_index) -> int64_t {
				int from_node = manager.IndexToNode(from_index).value();
				int to_node = manager.IndexToNode(to_index).value();
				return data.distance_matrix[from_node][to_node];
			});

	routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

	const int demand_callback_index = routing.RegisterUnaryTransitCallback(
			[&data, &manager](int64_t from_index) -> int64_t {
				// Convert from routing variable Index to demand NodeIndex.
				int from_node = manager.IndexToNode(from_index).value();
				return data.demands[from_node];
			});
	routing.AddDimensionWithVehicleCapacity(demand_callback_index, // transit callback index
			int64_t { 0 },               // null capacity slack
			data.vehicle_capacities,  // vehicle maximum capacities
			true,                     // start cumul to zero
			"Capacity");

	RoutingSearchParameters search_parameters =
			DefaultRoutingSearchParameters();
	search_parameters.set_first_solution_strategy(
			FirstSolutionStrategy::PATH_CHEAPEST_ARC);
	search_parameters.set_local_search_metaheuristic(
			LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
	search_parameters.mutable_time_limit()->set_seconds(2);

	const Assignment *solution = routing.SolveWithParameters(search_parameters);

	PrintSolution(data, manager, routing, *solution);
}
}

int main(int argc, char **argv) {
	std::string s1, s2, s3;
	std::ifstream File;
	int N, V;
	double c;
	N = V = c = 0;

	File.open(argv[1]);

	double t;
	File >> s1 >> s2 >> s3;
	std::istringstream os1(s1);
	os1 >> N;
	std::istringstream os2(s2);
	os2 >> V;
	std::istringstream os3(s3);
	os3 >> c;

	//std::cout << N << " " << V << " " << c << std::endl;

	vector<double> X(N), Y(N);
	vector<int64_t> D(N);
	vector<int64_t> vehicle_capacities(V, c);

	for (int i = 0; i < N; ++i) {
		File >> s1 >> s2 >> s3;
		std::istringstream os12(s1);
		os12 >> D[i];
		std::istringstream os22(s2);
		os22 >> X[i];
		std::istringstream os32(s3);
		os32 >> Y[i];
	}

	std::cout << "Calculating distances..." << std::endl;
	vector<vector<double> > map(N, vector<double>(N, 0));
	for (int i = 0; i < N; ++i) {
		for (int j = i + 1; j < N; ++j) {
			map[i][j] = map[j][i] = AJ*sqrt(pow(X[i] - X[j], 2) + pow(Y[i] - Y[j], 2));
		}
	}
	cout << "Finished calculating distances.\n";
	
	operations_research::DataModel data;
	data.demands = D;
	data.distance_matrix = map;
	data.num_vehicles = V;
	data.vehicle_capacities = vehicle_capacities;

	operations_research::VrpCapacity(data);
	return EXIT_SUCCESS;
}
