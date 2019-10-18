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

TaskStatus Next(TaskStatus task_status)
{// Выделим в отдельную функцию получение следующего по приоритету типа задачи
	return static_cast<TaskStatus>(static_cast<int>(task_status) + 1);
}

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
		TasksInfo updated, not_updated, summary; // карты обновленных, необновленных задач, и их сумма
		//чтобы не выделять память под summary можно в конце сделать цикл обновления tasks_map по updated + not_updated
		//либо делать вектор для сохранения нулевых ключей и цикл очистки нулей + цикл добавления значений updated (либо сделать функцию)
		TasksInfo& tasks_map = team[person]; // ссылка на карту заданий данного сотрудника
		for (TaskStatus task_code = TaskStatus::NEW; task_code < TaskStatus::DONE; task_code = Next(task_code))
		{// итерируемся по кодам заданий
			if (task_count > 0)
			{
				updated[Next(task_code)] = min(task_count, tasks_map[task_code]); // занесли в обновленный минимальное значение
				summary[Next(task_code)] = updated[Next(task_code)]; // занесли в суммарный
				tasks_map[task_code] -= updated[Next(task_code)]; // получаем либо 0 либо остаток операций
				task_count -= updated[Next(task_code)]; // уменьшаем счетчик
			}
			if (task_count == 0)
			{ // если обновили не все задачи типа task.first
				not_updated[task_code] = tasks_map[task_code]; // занести все в not_updated
				summary[task_code] += tasks_map[task_code];  // дополняем суммарную карту
			}
		} // имеет готовый результат
		summary[TaskStatus::DONE] += tasks_map[TaskStatus::DONE]; // забираем старые выполненные задачи из старой карты
		tasks_map = std::move(summary); // меняем исходную карту на актуальную
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
