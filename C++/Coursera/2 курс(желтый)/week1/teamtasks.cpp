#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <tuple>

using namespace std;
// Перечислимый тип для статуса задачи
enum class TaskStatus { NEW, IN_PROGRESS, TESTING, DONE };

// Объявляем тип-синоним для map<TaskStatus, int>, позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

void PrintTasksInfo(TasksInfo tasks_info)
{ // печать состаяния заданий
	cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
		", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
		", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}
class TeamTasks
{
private:
	map<string, TasksInfo> team; // карта сотрудник - список задач
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const
	{ // гарантируется, что метод GetPersonTasksInfo не будет вызван для разработчика, не имеющего задач
		return team.at(person);
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person)
	{
		++team[person][TaskStatus::NEW];
	}

	// Обновить статусы по данному количеству задач конкретного разработчика, подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
	{// Гарантируется, что task_count является положительным числом
		TasksInfo updated, not_updated; // список обновленных и необновленных задач
		vector<TaskStatus> for_delete;
		for (auto& task : team[person]) // ПРОВЕРЬ НА task_count > size!!!!!!!!!!!!!!
		{// пары задание - количествоэ
			if (task.first != TaskStatus::DONE)
			{
				if (task_count > 0)
				{ // проверяем только невыполненные
					int how_much = (task.second < task_count) ? task.second : task_count; // сколько вычитаем (либо все задачи, либо часть)
					task.second -= how_much; // вычитаем нужное количество
					if (task.second == 0)
					{ // если элементов не осталось, заносим ключ в список на удаление
						for_delete.push_back(task.first);
					}
					task_count -= how_much; // уменьшаем счетчик
					updated[static_cast<TaskStatus>(static_cast<int>(task.first) + 1)] = how_much; // занесли в обновленный
				}
				if (task_count == 0)
				{ // если обновили не все задачи типа task.first
					not_updated[task.first] = task.second; // занести все в not_updated
				}
			}
		} // имеет готовый результат, нужно изменить team[person]
		for (const auto& key_to_delete : for_delete)
		{ // очищаем ненужные ключи
			team[person].erase(key_to_delete);
		}
		for (const auto& pair_to_add : updated)
		{ // заносим обновленные ключи
			team[person][pair_to_add.first] += pair_to_add.second;
		}
		return tuple(updated, not_updated);
	}
};

int main()
{
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	return 0;
	/*
	Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
	Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
	Updated Ivan's tasks: 0 new tasks, 2 tasks in progress, 0 tasks are being tested, 0 tasks are done
	Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
	Updated Ivan's tasks: 0 new tasks, 1 tasks in progress, 1 tasks are being tested, 0 tasks are done
	Untouched Ivan's tasks: 0 new tasks, 1 tasks in progress, 0 tasks are being tested, 0 tasks are done
	*/
}
