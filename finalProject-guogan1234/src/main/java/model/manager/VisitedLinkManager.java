package model.manager;

import data.VisitedLink;
import model.VisitedLinkModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * A class manages database operations about visited link
 */
public class VisitedLinkManager implements VisitedLinkModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public VisitedLinkManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a visited link
     * @param visitedLink A VisitedLink class contains visited link info
     * @return The operated row count
     */
    @Override
    public int add(VisitedLink visitedLink) {
        if(visitedLink == null) return -1;
        String sql = "insert into visit_link(user, link, dateString) values(?, ?, ?)";
        return jdbcTemplate.update(sql, visitedLink.getUser(), visitedLink.getLink(), visitedLink.getDate());
    }

    /**
     * Find visited links by user
     * @param user user
     * @return visited links with specific username
     */
    @Override
    public List<VisitedLink> findByUser(String user) {
        if(user == null) return null;
        String sql = "select * from visit_link where user = ?";
        return (List<VisitedLink>) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                List<VisitedLink> visitedLinks = new ArrayList<>();
                while (resultSet.next()){
                    VisitedLink visitedLink = new VisitedLink();
                    visitedLink.setId(resultSet.getInt(1));
                    visitedLink.setUser(resultSet.getString(2));
                    visitedLink.setLink(resultSet.getString(3));
                    visitedLink.setDate(resultSet.getString(4));
                    visitedLinks.add(visitedLink);
                }
                return visitedLinks;
            }
        }, user);
    }

    /**
     * Delete records of visited links
     * @param user user
     * @return The operated row count
     */
    @Override
    public int deleteByUser(String user) {
        if(user == null) return -1;
        String sql = "delete from visit_link where user = ?";
        return jdbcTemplate.update(sql, user);
    }
}
