---
sidebar_navigation:
  title: FAQ
  priority: 001
description: Frequently asked questions regarding projects
robots: index, follow
keywords: projects FAQ, project questions
---

# Frequently asked questions (FAQ) for projects

### How can I get an overview over multiple projects at the same time?

There are several possibilities:

1. To see only the projects without their work packages go to the [Projects overview](../#view-all-projects) ("View all projects")
2. To see all work packages go to the work package module and set the filter "Subproject". Alternatively you can click on *Modules ->Work packages* in the upper right hand corner to access the [global work packages list](../#global-work-packages-list).
3. Go to the Project overview of a project with subprojects, add the widget "Work package table" and set the filter mentioned above. Find more information on this topic [here](../../project-overview/#add-a-widget-to-the-project-overview). Additionally you could add the column "Progress".
4. Add the widget "Work package table" to your My Page and set the filter mentioned above. Find more information on this topic [here](../../../getting-started/my-page/#configure-the-my-page). Additionally you could add the column "Progress".

We will introduce further similar functions in the course of implementing multi-project management, this is planned for the next few years. 

## What is the difference between creating a project from a template and copying the template project?

Creating a project from a template and copying projects serve slightly different purposes: Project templates provide an easy way to create a new project while copying all the data (as far as supported) of the source project.
Copying projects provides more flexibility: You can choose which data to copy from the source project. Please note that choosing not to copy certain project data may lead to errors (e.g. when work packages are assigned to users who are not copied along with the project).

### We have different departments in our company and need projects by departments. Can I use sub-projects for the departments?

Yes, that is in most cases the best solution.

## How are the Backlogs module, boards and versions related? Can I use boards with versions?

In OpenProject, you can work agilely according to Scrum (backlogs) or Kanban (boards). Versions in OpenProject represent a "container" that contains the work packages to be processed.
Versions serve a double function: On the one hand, you can use them to plan your product releases, and on the other hand, you can use them to map the product backlog(s) and sprints required for Scrum.
As soon as you have created at least one version in a project, the entry "Roadmap" is displayed on the left side in your project, which you can use to get an overview of the versions (intended primarily for releases).
The [Backlogs module](../../backlogs-scrum) uses versions to map the product backlog or sprints. By using the backlog, however, some special rules occur: For example, tasks must be assigned to the same version as the associated (parent) work packages. 
If you do not work according to Scrum I would recommend to deactivate the Backlogs module and use the [Boards module](../../agile-boards) instead. If you have activated the boards module you can create a version board. You can find an example [here](https://community.openproject.com/projects/openproject/boards/2077).